#include "TaskScheduler.h"

void emptyTask() {
  // Empty 
} 

String getTime(unsigned long ms) {
  int minutes = ms / 60000;
  int seconds = (ms % 60000) / 1000; 
  int msRemaining = (ms % 60000) % 1000;
  
  char buffer[20];
  sprintf(buffer, "%d:%05.2f", minutes, seconds + (float)msRemaining/1000);
  
  return String(buffer);  
}

void setup() {

  Serial.begin(115200);
  Sched scheduler;
  
  // Create 50 tasks
  for(int i=0; i<50; i++) {
   int randomInterval = random(100, 5000);
   Task t(emptyTask, randomInterval); 
   scheduler.addTask(&t);   
  } 

  unsigned long startTime = millis();
  
  while(millis() - startTime < 120000) {

    scheduler.run();
    
    unsigned long elapsedMs = millis() - startTime; 
    
    // Print statistics every 10 seconds 
    if(elapsedMs % 10000 == 0) {
      String elapsedTime = getTime(elapsedMs);
      

      int enabledTasks = 0;
      int totalTasks = scheduler.getSize();
      Task* tx = scheduler.getTasks();

      for(Task* t : scheduler.tTasks) {
        if(t->isEnabled()) {
          enabledTasks++;  
        }
      }

      Serial.printf("Elapsed: %s, Enabled: %d, Total: %d\n", 
                    elapsedTime.c_str(), enabledTasks, totalTasks);  
    }
  
  }

}

void loop() {
  
}
