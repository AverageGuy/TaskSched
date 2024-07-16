/*
 * Serial.println{"Attempting connect to 
  -----------------------
  Demo OTA based on ElegantOTA - Demo Example
  -----------------------

  Skill Level: Beginner

  This example provides with a bare minimal app with ElegantOTA functionality.

  Github: https://github.com/ayushsharma82/ElegantOTA
  WiKi: https://docs.elegantota.pro

  Works with following hardware:
  - ESP8266
  - ESP32
  - RP2040 (with WiFi) (Example: Raspberry Pi Pico W)


  Important note for RP2040 users:
  - RP2040 requires LittleFS partition for the OTA updates to work. Without LittleFS partition, OTA updates will fail.
    Make sure to select Tools > Flash Size > "2MB (Sketch 1MB, FS 1MB)" option.
  - If using bare RP2040, it requires WiFi module like Pico W for ElegantOTA to work.

  -------------------------------

  Upgrade to ElegantOTA Pro: https://elegantota.pro
*/

/**
  TemplateDemo

 Author: Jim Lynch
*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
#elif defined(TARGET_RP2040)
  #include <WiFi.h>
  #include <WebServer.h>
#endif

#include <ElegantOTA.h>
#include <TaskSched.h>
#include <TelnetSpy.h>
#include <PubSubClient.h>

const char* ssid = "Your_SSID";
const char* password = "Your_Password";

const char* mqtt_server = "mosquitto.org";
#if defined(ESP8266)
  ESP8266WebServer server(80);
#elif defined(ESP32)
  WebServer server(80);
#elif defined(TARGET_RP2040)
  WebServer server(80);
#endif

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
Task* tStart=new Task(connectNet, 100, true, 1, "Conn", true);
/** start with this task, run immediately, task is enabled but run only once. */
Task* tWait=new Task(waitForConn, 2500, false, 20, "Wait", false);
/** This task is kicked off by tStart to wait for a connect. */
Task* tConnMQTT = new Task(connectMQTT, 100, false, 1, "ConnMQTT", true);
/** If we got connected then this task is run once to connect to MQTT */
Task* tWaitMQTT=new Task(waitMQTT, 500, false, 20, "Run", false);
/** This is run by the tConnMQTT to wait for a connect.  See tWait above */
Task* tRun=new Task(runForever, 10000, false, 0, "Run", true);
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
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}


void connectNet() {
    delay(500);
    Serial.printf("Attempting connection to network, %s\n",ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    SERIAL.println("");
    tWait->enable();
    Serial.println("Pass control to waitForConn");
}

void waitForConn(Task *task) {
    Serial.println("WaitForConn running");
    if(WiFi.status() == WL_CONNECTED) 
    {
        Serial.println("Connected");
        task->disable();
        Serial.println("Disabling");
        tConnMQTT->enable();
        SERIAL.println("");
        SERIAL.print("Connected to ");
        SERIAL.println(ssid);
        SERIAL.print("IP address: ");
        SERIAL.println(WiFi.localIP());

        server.on("/", []() {
                server.send(200, "text/plain", "Hi! I am ESP8266.");
                });

        ElegantOTA.begin(&server);    // Start ElegantOTA
        ElegantOTA.onStart(onOTAStart);
        ElegantOTA.onEnd(onOTAEnd);
        server.begin();
        SERIAL.println("HTTP server started");
        SerialAndTelnet.setWelcomeMsg("Welcome to the TelnetSpy example\n\n");
        SerialAndTelnet.setCallbackOnConnect(telnetConnected);
        SerialAndTelnet.setCallbackOnDisconnect(telnetDisconnected);
        SERIAL.println("Start serial now ");
        tConnMQTT->enable();
        Serial.println("Enable connMQTT");
    } else {
        SERIAL.print(".");
        if(task->isLastIteration()) {
            SERIAL.println("Can't connect to the network.  Trying again in 10 minutes.");
            task->disable();
            task->showInit();
            
            tStart->restart();
            tStart->setInterval(10*TASK_MINUTE);
            tStart->setIterations((1));
            tStart->enable();
            tStart->setImmediately(false);
            task->showInit();
        }
    }
}

void connectMQTT(Task *task) {
    client.connect("ESP8266Client");
    Serial.println("Attempting connect to mqtt server");
    tWaitMQTT->enable();
    task->disable();
}

void waitMQTT(Task *task) {
    if(client.connected()) {
        Serial.println("Got mqtt connection");
        char ip[40];  // 15 chars for IP (max: xxx.xxx.xxx.xxx) + null terminator plus a bit for time stamp
        IPAddress ipAddr = WiFi.localIP();
        sprintf(ip, "%d.%d.%d.%d after %ld ms", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3],millis());
        Serial.println(ip);
        client.publish("test/connection",ip);
        
        task->disable();
        tmqttLoop->enable();
        tRun->enable();
    } else {
        if(task->isLastIteration()) {
            SERIAL.println("Can't connect to the MQTT server.  Trying again in 10 minutes.");
            task->disable();
            tConnMQTT->restart();
            tConnMQTT->setInterval(10*TASK_MINUTE);
            tConnMQTT->setIterations((1));
            tConnMQTT->enable();
            tConnMQTT->setImmediately(false);
        }
    }
}

void runForever(Task *task) {
    if(WiFi.status() != WL_CONNECTED)  {
        /** We'll just start all over */
        SimpleList<Task *> taskList = scheduler.getTasks();
        for (auto it = taskList.begin(); it != taskList.end(); ++it) {
            // Other code...
            (*it)->restart();
            // More code...
        }
        return;
    }
    if(!client.connected()) { 
        Serial.println("mqtt server not connected.");
        /** try to reconnect */
        tConnMQTT->restart();
        tConnMQTT->enable();
        Serial.printf("Starting reconnect at %ld\n",millis());
    }
    Serial.println("I'm running");
}

void setup(void) {
    Serial.begin(115200);
    delay(100);
    Serial.println("Starting scheduler\n");
    scheduler.addTask(tStart);
    scheduler.addTask(tWait);
    scheduler.addTask(tConnMQTT);
    scheduler.addTask(tWaitMQTT);
    scheduler.addTask(tRun);

    // Wait for connection
    scheduler.begin();
    tStart->showInit();
    Serial.println("After showInit");
}

void mqttLoop(Task *task) {
    if(!client.connected()) { 
        Serial.println("mqtt server not connected.");
        /** try to reconnect */
        tConnMQTT->restart();
        tConnMQTT->enable();
        Serial.printf("Starting reconnect at %ld\n",millis());
        task->restart();
    } else {
        client.loop();
    }
}

void loop(void) {
  server.handleClient();
  ElegantOTA.loop();
    SerialAndTelnet.handle();
    scheduler.run();
    client.loop();
}
