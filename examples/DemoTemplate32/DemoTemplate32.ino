
/**
  TemplateDemo for ESP32

 Author: Jim Lynch
 Adapted for ESP32
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>
#include <TaskSched.h>
#include <TelnetSpy.h>
#include <PubSubClient.h>

const char* ssid = "Your_SSID";
const char* password = "Your_Password";

const char* mqtt_server = "mosquitto.org";
WebServer server(80);

WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, espClient); // 1883 is the listener port for the Broker

TelnetSpy SerialAndTelnet;

Sched scheduler;
unsigned long ota_progress_millis = 0;

#define SERIAL  SerialAndTelnet

void connectNet();
void waitForConn(Task *task);
void connectMQTT(Task *task);
void waitMQTT(Task *);
void runForever(Task *task);
void mqttLoop(Task *task);

Task* tStart = new Task(connectNet, 100, true, 1, "Conn", true);
/** start with this task, run immediately, task is enabled but run only once. */
Task* tWait = new Task(waitForConn, 2500, false, 20, "Wait", false);
/** This task is kicked off by tStart to wait for a connect. */
Task* tConnMQTT = new Task(connectMQTT, 100, false, 1, "ConnMQTT", true);
/** If we got connected then this task is run once to connect to MQTT */
Task* tWaitMQTT = new Task(waitMQTT, 500, false, 20, "Run", false);
/** This is run by the tConnMQTT to wait for a connect.  See tWait above */
Task* tRun = new Task(runForever, 10000, false, 0, "Run", true);
/** if everything is good, then maybe do something with the data, or not */
Task* tmqttLoop = new Task(mqttLoop, 100, false, 0, "MQTTLoop", true);
/** call client.loop frequently */

void telnetConnected() {
    SERIAL.println("Telnet connection established.");
}

void telnetDisconnected() {
    SERIAL.println("Telnet connection closed.");
}

void onOTAStart() {
  Serial.println("OTA update started!");
}

void onOTAProgress(size_t current, size_t final) {
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
}

void connectNet() {
    delay(500);
    Serial.printf("Attempting connection to network: %s\n", ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    tWait->enable();
}

void waitForConn(Task *task) {
    Serial.println("WaitForConn running");
    if(WiFi.status() == WL_CONNECTED) 
    {
        Serial.println("Connected");
        task->disable();
        
        SERIAL.println("");
        SERIAL.print("Connected to ");
        SERIAL.println(ssid);
        SERIAL.print("IP address: ");
        SERIAL.println(WiFi.localIP());

        server.on("/", []() {
            server.send(200, "text/plain", "Hi! I am ESP32.");
        });

        ElegantOTA.begin(&server);
        ElegantOTA.onStart(onOTAStart);
        ElegantOTA.onEnd(onOTAEnd);
        server.begin();
        SERIAL.println("HTTP server started");

        SerialAndTelnet.setWelcomeMsg("Welcome to the ESP32 TelnetSpy example\n\n");
        SerialAndTelnet.setCallbackOnConnect(telnetConnected);
        SerialAndTelnet.setCallbackOnDisconnect(telnetDisconnected);
        
        tConnMQTT->enable();
        Serial.println("Enable connMQTT");
    } else {
        SERIAL.print(".");
        if(task->isLastIteration()) {
            SERIAL.println("Can't connect to the network. Trying again in 10 minutes.");
            task->disable();
            
            tStart->setInterval(10 * TASK_MINUTE);
            tStart->setIterations(1);
            tStart->setImmediately(false);
            tStart->enable();
        }
    }
}

void connectMQTT(Task *task) {
    client.connect("ESP32Client");
    Serial.println("Attempting connect to mqtt server");
    tWaitMQTT->enable();
    task->disable();
}

void waitMQTT(Task *task) {
    if(client.connected()) {
        Serial.println("Got mqtt connection");
        char ip[64];
        IPAddress ipAddr = WiFi.localIP();
        sprintf(ip, "%d.%d.%d.%d after %ld ms", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3], millis());
        Serial.println(ip);
        client.publish("test/connection", ip);
        
        task->disable();
        tmqttLoop->enable();
        tRun->enable();
    } else {
        if(task->isLastIteration()) {
            SERIAL.println("Can't connect to the MQTT server. Trying again in 10 minutes.");
            task->disable();
            tConnMQTT->setInterval(10 * TASK_MINUTE);
            tConnMQTT->setIterations(1);
            tConnMQTT->setImmediately(false);
            tConnMQTT->enable();
        }
    }
}

void runForever(Task *task) {
    if(WiFi.status() != WL_CONNECTED)  {
        // Restart all logic or just wait for WiFi to reconnect
        return;
    }
    if(!client.connected()) { 
        Serial.println("mqtt server not connected.");
        tConnMQTT->enable();
    }
    Serial.println("I'm running");
}

void mqttLoop(Task *task) {
    if(!client.connected()) { 
        // Logic handled in runForever or here
    } else {
        client.loop();
    }
}

void setup(void) {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting ESP32 scheduler\n");
    
    scheduler.addTask(tStart);
    scheduler.addTask(tWait);
    scheduler.addTask(tConnMQTT);
    scheduler.addTask(tWaitMQTT);
    scheduler.addTask(tRun);
    scheduler.addTask(tmqttLoop);

    scheduler.begin();
}

void loop(void) {
    server.handleClient();
    ElegantOTA.loop();
    SerialAndTelnet.handle();
    scheduler.run();
}
