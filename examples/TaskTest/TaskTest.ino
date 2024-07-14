#include <Arduino.h>
#include <TaskSched.h>

void runTest(Task *);
SafePtr<Task> tstart=SafePtr<Task>(new Task(runTest, 500, true, 6, "First", true));
SafePtr<Task> t1;
SafePtr<Task> t2;
Sched scheduler;

int pass=0;
void runTest(Task *task)
{
    switch (pass) {
        case 0:
            
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;

    }
}

void first() {
}

void second(Task *task2) {
}

void setup() {
    Serial.begin(115200);
    delay(500);
    tstart=SafePtr<Task>(new Task(runTest, 500, true, 6, "First", true));
    t1=SafePtr<Task>(new Task(first, 700, false, 2, "First", false));
    t2=SafePtr<Task>(new Task(second, 1200, true, 1, "Second", false));
    scheduler.begin();

}

void loop() {
    scheduler.run();
}
