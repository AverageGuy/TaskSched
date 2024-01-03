#include <Arduino.h>
#include <TaskSched.h>

// Global variables to track task completion
bool task1Completed = false;
bool task2Completed = false;

// Global Sched object
Sched sched;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Define tasks in setup function
  Task* task1 = Task(1000, 5,  {
    // This task will execute every 1 second, 5 times in total
    Serial.println("Task 1 is running");
    task1Completed = true;
  });
  Task* task2 = Task(2000, 4,  {
    // This task will execute every 2 seconds, 4 times in total
    Serial.println("Task 2 is running");
    task2Completed = true;
  });
  Task* task3 = Task(6100, 1,  {
    // This task will execute once at the 6 second mark
    if (task1Completed) {
      Serial.println("Test Case Passed: Task 1 completed on time");
    } else {
      Serial.println("Test Case Failed: Task 1 did not complete on time");
    }
  });
  Task* task4 = Task(8.1, 1,  {
    // This task will execute once at the 8.1 second mark
    if (task2Completed) {
      Serial.println("Test Case Passed: Task 2 completed on time");
    } else {
      Serial.println("Test Case Failed: Task 2 did not complete on time");
    }
  });

  // Add tasks to Sched object
  sched.addTask(task1);
  sched.addTask(task2);
  sched.addTask(task3);
  sched.addTask(task4);
}

void loop() {
  // Execute Sched run method in loop function
  sched.run();
}

