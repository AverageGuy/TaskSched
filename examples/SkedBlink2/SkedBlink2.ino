#include <TaskSched.h>
int ledPin = LED_BUILTIN;
// ledPin = 13;
/**
 * Turns led on and off every  second for 20 iterations
 * It uses the setCallback of the Task class to swap the 
 * turnLedOn and turnLedOff functions.
 */


void dummy(Task *){
}
void  turnLedOn(Task * );
void  turnLedOff(Task * );
/** periodically turn the led on and then off
 */
Task* t = new Task(turnLedOn, 1000,true,20,"OnOff",true);
Task t1(dummy, 100, false, 20, "On1", false);
Task t2(dummy, 2.0, false, 20, "On2, false);
/** these last two tasks don't do anything, they are here to demonstrate
 * the two different constructors and what how they work.  
 * I e t1 specifies the interval as an integer (100) so the
 * interval will be 100 ms.  Task t2 specifies the interval
 * as a floating value (2.0) and that will be saved as
 * 2 seconds or 2000 ms.
 * 
 * The first method, Task* t = new Task(turnLedOn, 1000, true, 20, "OnOff",
 * true);, dynamically allocates memory for a new Task object using the new
 * operator. This creates the object on the heap, which is a region of
 * memory managed by the program's runtime environment. The t pointer then
 * stores the address of the newly created object. This approach is useful
 * when you need to control the object's lifetime and potentially
 * deallocate it later using the delete operator.
 * 
 * In contrast, the second method, Task t1(dummy, 100, false, 20, "On1",
 * false);, creates the object directly on the stack. The stack is a memory
 * region used for local variables and function calls. Objects on the stack
 * are automatically destroyed when they go out of scope, such as when the
 * function they're declared in returns. This method is often more
 * convenient when you know the object's lifetime is limited to the current
 * block of code and you don't need to manage its memory manually.
 * 
 * 
 * Key differences:
 * 
 * Memory allocation: Heap vs. stack
 * 
 * Lifetime control: Manual with new and delete vs. automatic
 * 
 * Pointer usage: Task* t vs. direct object Task t1
 * 
 * Flexibility: Dynamic allocation with new allows for greater flexibility
 * in object creation, storage, and management.
 * 
 * Efficiency: Stack allocation with Task t1 can be slightly more efficient
 * in terms of memory access and management. The choice of method depends
 * on the specific programming needs and memory management considerations
 * of the application.
 * 
*/
// Heads up! 
// The way the built-in LEDs are wired might be different between boards.
// This code assumes a setup like the WEMOS D1 mini (clone). If you're using
// a different board, you might need to flip the ON/OFF logic for the LEDs 
// to match the actual behavior you see.
//
void turnLedOn(Task *ta) {
    digitalWrite(ledPin, LOW); 
    t->setCallback(turnLedOff);
    t->setName("Off");
    Serial.print("ON ");
}

void turnLedOff(Task *ta) {
    digitalWrite(ledPin, HIGH);
    t->setCallback(turnLedOn);
    t->setName("On");
    Serial.println("OFF ");
}

Sched scheduler;  

void setup() {


    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    scheduler.addTask(t);
    scheduler.addTask(&t1);
    scheduler.addTask(&t2);
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
            if(teststr.startsWith("status")) {
                String str = scheduler.displayStatus(true);
                Serial.print(str);
            } 
        }
    }
}
