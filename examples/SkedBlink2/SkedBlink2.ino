#include <TaskSched.h>
int ledPin = 13;

void turnLedOn() {
  digitalWrite(ledPin, HIGH); 
  // Rest of logic
}

void turnLedOff() {
  digitalWrite(ledPin, LOW);
  // Rest of logic  
}

Sched scheduler;  

void setup() {

  Task t(turnLedOn, 2000);
  
  scheduler.addTask(&t);
  scheduler.begin();
}

void loop() {
  scheduler.run();
}
