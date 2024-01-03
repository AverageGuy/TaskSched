#include <Arduino.h>
#include <TaskSched.h>
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

//int ledPin = LED_BUILTIN;
int ledPin = 13; // this worked for my ttgo esp32
void turnLedOn();
void turnLedOff(Task*);

Sched scheduler; 

  Task* t1 = new Task(turnLedOn, 2000, true, 20, "OnTask", true);
  Task* t2 = new Task(turnLedOff, 1000, true, 20, "OffTask",false); 
void turnLedOn() {
  digitalWrite(ledPin, HIGH); 
}

void turnLedOff(Task *tsk) {
  digitalWrite(ledPin, LOW);  
  t2->setInterval(2000);
  Serial.print("Interval is ");
  Serial.println(tsk->getInterval());
}

void setup() {


    Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  scheduler.addTask(t1);
  scheduler.addTask(t2);
  
  scheduler.begin();
}

void loop() {

  scheduler.run();

  // Validate LED blinks on even seconds
  // and off on odd seconds for 20 iterations

}
