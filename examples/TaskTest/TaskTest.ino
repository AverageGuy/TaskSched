#include <Arduino.h>
#include <TaskSched.h>

class TaskTest  {
 public:
  TaskTest() {
    // Create a task
    Task* task = new Task();

    // Enable the task
    task->enable();

    // Check that the task is enabled
    assertTrue(task->isEnabled());

    // Disable the task
   task->disable();

    // Check that the task is disabled
    assertFalse(task->isEnabled());

    // Delete the task
    delete task;
  }
setup() {
  Serial.begin(115200);
}

void loop() {
  Test::run(new TaskTest());
}
