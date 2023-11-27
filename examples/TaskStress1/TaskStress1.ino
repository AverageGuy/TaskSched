#include "TaskSched.h"

unsigned long startTime; 
String elapsedTime;

void setup() {

  startTime = millis();
  
  Sched scheduler;
  // Create tasks

  while(millis() - startTime < 120000) {
    
    elapsedTime = getTime(millis() - startTime);  

    scheduler.run();
    
    // Rest of logic    
    Serial.printf("Elapsed: %s", elapsedTime.c_str());
  }

}

void loop() {

}
