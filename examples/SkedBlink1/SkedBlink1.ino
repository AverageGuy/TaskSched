#include <Arduino.h>
#include <TaskSched.h>
// Test LED Blink Tasks

int ledPin = LED_BUILTIN;
void turnLedOn();
void turnLedOff();
Task* t4 = new Task(turnLedOn, 1000, true, 20, "OnTask", true);
Task t3(turnLedOff, 1000, true, 20, "OffTask"); 

Sched scheduler; 

void turnLedOn() {
  digitalWrite(ledPin, HIGH); 
}

void turnLedOff() {
  digitalWrite(ledPin, LOW);  
}

void setup() {

  pinMode(ledPin, OUTPUT);
  

  Task* t5 = new Task(turnLedOn, 1000, true, 20, "OnTask", true);
  Task t1(turnLedOn, 1000, true, 20, "OnTask", true);
  Task t2(turnLedOff, 1000, true, 20, "OffTask"); 

  scheduler.addTask(&t1);
  scheduler.addTask(&t2);
  
  scheduler.begin();
}

void loop() {

  scheduler.run();
  delay(1000);

  // Validate LED blinks on even seconds
  // and off on odd seconds for 20 iterations

}
