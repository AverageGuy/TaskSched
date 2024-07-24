#include <TaskSched.h>
#ifdef LED_BUILTIN
int ledPin = LED_BUILTIN;
#else
int ledPin =2;
#endif
/** if LED_BUILTIN isn't defined for your board, you will have to set it
 * manually. */

/** On my ESP32 test board, the function of the led is inverted so 
 * when it thought it was turning the led on it was actually turning it off.
 */

/**
 * Turns led on and off every  second for 20 iterations
 * It uses the setCallback of the Task class to swap the 
 * turnLedOn and turnLedOff functions while running only one task
 */

int count=0;
void dummy(Task * );
void  turnLedOn(Task * );
void  turnLedOff(Task * );
/** periodically turn the led on and then off
 */
Task *t;
Task *t1;
Task *t2;
void dummy(Task *ta){
    if(ta->isFirstIteration()) {
        Serial.printf("At count = %d, this is the first iteration\n",count);
    }
    count++;
    Serial.println("Dum");
    if(ta->isLastIteration()) {
        Serial.printf("Count = %d\n",count);
        Serial.printf("1 This is the last iteration\n");
    }
}
void turnLedOn(Task *ta) {
    count++;
    digitalWrite(ledPin, LOW); 
    t->setCallback(turnLedOff);
    t->setName("Off");
    Serial.print("ON ");
    if(ta->isLastIteration()) {
        Serial.printf("Count = %d\n",count);
        Serial.println("2 This is the last iteration\n");
        t1->enable();
        count=0;
    }
}


void turnLedOff(Task *ta) {
    count++;
    digitalWrite(ledPin, HIGH);
    t->setCallback(turnLedOn);
    t->setName("On");
    Serial.println("OFF ");
    if(ta->isLastIteration()) {
        Serial.printf("Count = %d\n",count);
        Serial.printf("1 This is the last iteration\n");
        t1->enable();
        count=0;
    }
}

Sched scheduler;  

void setup() {

    t = new Task(turnLedOn, 1000,true,20,"OnOff",true);
    t1= new Task(dummy, 1000, false, 4, "On1", false);
    t2= new Task(dummy, 2.0*TASK_SECOND, false, 20, "On2", false);
    /** t2 doesn't do anything, it is here are here to demonstrate
     * the two different constructors and what how they work.  
     * I e t1 specifies the interval as an integer (100) so the
     * interval will be 100 ms.  Task t2 specifies the interval
     * as a floating value (2.0) and that will be saved as
     * 2 seconds or 2000 ms.
     * There is another macro TASK_MINUTE that can be used to convert minutes
     * to ms. E. g interval = 1.5*TASK_MINUTE will set the interval to 1.5 minutes.
     * The t1 task simply test to be sure that the task stops after the count
     * is done.  Both tasks demonstrate the use of the isLastIteration()) 
     * method.
     * The all the methods dynamically allocate memory for a new Task object
     * using the new operator. 
     * This creates the object on the heap, which is a region of
     * memory managed by the program's runtime environment. The t pointer then
     * stores the address of the newly created object. This approach is useful
     * when you need to control the object's lifetime and potentially
     * deallocate it later using the delete operator. But you don't have to
     * because it's a smart pointer and will delete the task when the pointer 
     * goes out of scope.
     * 
     */
    // Heads up! 
    // The way the built-in LEDs are wired might be different between boards.
    // This code assumes a setup like the WEMOS D1 mini (clone). If you're using
    // a different board, you might need to flip the ON/OFF logic for the LEDs 
    // to match the actual behavior you see.
    //
    Serial.begin(115200);
    delay(500);
    Serial.println("");
    pinMode(ledPin, OUTPUT);
    scheduler.addTask(t);
    scheduler.addTask(t1);
    scheduler.addTask(t2);
    scheduler.begin();
    Serial.println("Begin");
}

void loop() {
    char *token;
    char* ptr[5];
    const char *delim = " ";
    int tokens=0;
    char* buf = NULL;
    scheduler.run();
    if(Serial.available()) {
        String teststr = Serial.readString();
        int len=teststr.length();
        if(len<100) {// if >100 It is garbage ignore
            buf = new char[len+1];
            strcpy(buf,teststr.c_str());
            token = strtok(buf,delim);
            ptr[0]=token;
            tokens=1;
            for(;tokens<5;++tokens) {
                ptr[tokens]=strtok(NULL,delim);
                if(!ptr[tokens]) { // is NULL
                    break;
                }
            }
            Serial.printf("%ld Found %s\n",__LINE__,teststr.c_str());
            /** To demonstrate these two operations, you'll need to 
             * send 'status' or 'list' to the serial port.  
             * How that is accomplished is beyond the scope of this
             * comment */
            if(teststr.startsWith("status")) {
                const SimpleList<Task *>& tTasks = scheduler.getTasks();
                for (const auto* task : tTasks) {
                    String info = task->showTaskInfo();
                    Serial.print(info);
                }
            } else if(teststr.startsWith("list")) {
                /** Here are two ways of enumerating the task list 
                 * The first way uses the list iterator. */
                Serial.println("This output was produced by asking the getTasks function\nto provide a list of the tasks.  It then prints the name\nof each task.");
                const SimpleList<Task *>& tTasks = scheduler.getTasks();
                for (const auto* task : tTasks) {
                    String name = task->getName();
                    Serial.printf("Name=%s\n",name.c_str());
                }
                /** The second way uses the read method of the SimpleList class 
                 * If you run this twice, you'll notice that the second time
                 * this loop doesn't ouput anything.  That's because 
                 * the next read position is at the end.  Use the rewind
                 * method to reset the pointer. */
                Serial.println("This output was produced by using the read function of the SimpleList class\nto return a task.  We use the task getName function to print the name.");
                while(Task *task = tTasks.read()) {
                    String name = task->getName();
                    Serial.printf("Name=%s\n",name.c_str());
                }
            }
        }
    }
}
