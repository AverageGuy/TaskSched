#include <Arduino.h>
#include <TaskSched.h>
#include <SafePtr.h>
/** Test LED Blink Tasks
 * You may have to adjust the ledPin for your dev board
 * It blinks the led on and off every second. For 20 iterations
 * Or 40 seconds.
 * It show only the most basic tasks.  The first task is run immediately 
 * and runs for 2 seconds and every two seconds after that for 20 iterations.
 * The second task runs for 20 iterations but starts 
 * at 1 second into the run. It then increases the interval to 2 seconds so it
 * alternates with the first task to toggle the led.
 * This example shows how to change the interval on the fly and also
 * fetch and display the interval.
 */

int ledPin = LED_BUILTIN;
void turnLedOn(Task*);
void turnLedOff(Task*);

Sched scheduler; 

SafePtr<Task> t1;
SafePtr<Task> t2;

void turnLedOn(Task *tsk) {
    Serial.println("Turn on");
    digitalWrite(ledPin, HIGH); 
    String res;
    res = tsk->formatMS(millis());
    Serial.printf("OnTime: %s\n",res.c_str());

}

void turnLedOff(Task *tsk) {
    Serial.println("Turn off");
    String res;
    res = tsk->formatMS(millis());
    Serial.printf("OffTime: %s\n",res.c_str());
    digitalWrite(ledPin, LOW);  
    t2->setInterval(2000);
}
unsigned long start;
unsigned long elapsed;
unsigned long limit = 5000;

void setup() {


    ledPin = LED_BUILTIN; // Works with some boards, not all 
    Serial.begin(115200);
    delay(500); // wait for serial
    pinMode(ledPin, OUTPUT);
    t1 = SafePtr<Task>(new Task(turnLedOn, 2000L, true, 20, "OnTask", true));
    t2 = SafePtr<Task>(new Task(turnLedOff, 1000L, true, 20, "OffTask", false));
    scheduler.addTask(t1);
    scheduler.addTask(t2);
    Serial.println("");
    Serial.println("Starting");
    scheduler.begin();
    String str = scheduler.displayStatus(true);
    Serial.print(str);


    start=millis();
}

void loop() {

    scheduler.run();

    // Validate LED blinks on even seconds
    // and off on odd seconds for 20 iterations
}
