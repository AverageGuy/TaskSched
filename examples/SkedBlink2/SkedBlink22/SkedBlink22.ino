#include <TaskSched.h>
int ledPin = 13;
/**
 * Turns led on and off every  second for 20 iterations
 * It uses the setCallback of the Task class to swap the 
 * turnLedOn and turnLedOff functions.
 */

void turnLedOff();
void turnLedOn();

template<typename T>
void turnLedOn(Task<T>* t);
void turnLedOff(Task<T>* t);
Task<T> t1(turnLedOn, 1000);
Task<T>* t = new Task(turnLedOn, 1000,true,20,"OnOff",true);
void turnLedOn() {
    digitalWrite(ledPin, HIGH); 
    t->setCallback(turnLedOff);
    t->setName("Off");
    Serial.print("ON ");
}

void turnLedOff() {
    digitalWrite(ledPin, LOW);
    t->setCallback(turnLedOn);
    t->setName("On");
    Serial.println("OFF ");
}

Sched scheduler;  

void setup() {


    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    scheduler.addTask(t);
    scheduler.begin();
    Serial.println("Begin");
}

void loop() {
    scheduler.run();
}
