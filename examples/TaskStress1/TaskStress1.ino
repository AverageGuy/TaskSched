/**
 * This example shows how you might run the scheduler in a timer loop
 * rather than running it in the main loop.  It also shows how to loop 
 * through the list of tasks that is being scheduled.  Since the program
 * runs forever the generated tasks are never deleted in this example
 *
 * The output looks like:
 * 
The 233rd prime number is 1471
Elapsed: 0:10.00, Enabled: 51, Total: 51
The 511st prime number is 3659
Elapsed: 0:20.20, Enabled: 51, Total: 51
The 767th prime number is 5843
Elapsed: 0:30.40, Enabled: 51, Total: 51
The 1042nd prime number is 8297
Elapsed: 0:40.60, Enabled: 51, Total: 51
The 1311st prime number is 10753
Elapsed: 0:50.80, Enabled: 51, Total: 51
 * Note that because we are using a long delay, 200ms between 
 * loops the report runs every 10.2 seconds.  If the scheduler were
 * to run from the loop rather than the TickTwo timer, the error 
 * would be significantly less.  So don't use the timer if you need
 * accuate timing.  If you need really accuate timeing, don't use
 * this library since it doesn't compensate for long running tasks
 * and can get behind.
 */
#include "TaskSched.h"
#include "TickTwo.h"
#include <bits/stdc++.h>
using namespace std;

unsigned long startTime; 
String elapsedTime;
Sched scheduler;
unsigned long elapsedMs;
void runLoop();
unsigned long loopRun=0;
TickTwo runTimer(runLoop, 200, 0, MILLIS);

unsigned long prime=0;
unsigned long primeCount=0;

#include <Arduino.h>

String printNumberSuffix(unsigned long number) {

  int lastDigit = number % 10;
  
  String suffix;
  if(lastDigit == 1 && number != 11) {
    suffix = "st";
  } else if(lastDigit == 2 && number != 12) {
    suffix = "nd";    
  } else if(lastDigit == 3 && number != 13) {
    suffix = "rd";
  } else {
    suffix = "th";
  }

  String numberString = String(number);
  numberString += suffix;

  return numberString;
}

// Function that returns true if n 
// is prime else returns false 
bool isPrime(unsigned long n) 
{ 
    // Corner cases 
    if (n <= 1)  return false; 
    if (n <= 3)  return true; 
   
    // This is checked so that we can skip  
    // middle five numbers in below loop 
    if (n%2 == 0 || n%3 == 0) return false; 
   
    for (unsigned long i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return false; 
   
    return true; 
} 

int nextPrime(unsigned long N)
{
 
    // Base case
    if (N <= 1)
        return 2;
 
    unsigned long prime = N;
    bool found = false;
 
    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;
 
        if (isPrime(prime))
            found = true;
    }
 
    primeCount++;
    return prime;
}

String getTime(unsigned long ms) {
    int minutes = ms / 60000;
    int seconds = (ms % 60000) / 1000; 
    int msRemaining = (ms % 60000) % 1000;

    char buffer[20];
    sprintf(buffer, "%d:%05.3f", minutes, seconds + (float)msRemaining/1000);

    return String(buffer);  
}

void emptyTask() {
    // Empty 
    prime=nextPrime(prime);
} 

void reportTask()
{
    String elapsedTime = getTime(elapsedMs);


    int enabledTasks = 0;
    int totalTasks = scheduler.getSize();
    list<Task*>::iterator it;
    list<Task*> tTasks = scheduler.getTasks();
    Serial.printf("The %s prime number is %ld\n",printNumberSuffix(primeCount+1),prime);

    for (it = tTasks.begin(); it != tTasks.end(); ++it){
        Task *currentTask = *it;
        if(currentTask->isEnabled()) {
            enabledTasks++;  
        }
    }

    Serial.printf("Elapsed: %s, Enabled: %d, Total: %d\n", 
            elapsedTime.c_str(), enabledTasks, totalTasks);  
}

Task report(reportTask,10000,true,0,"Report");
//                      Task(turnLedOn, 1000,true,20,"OnOff",true);
void setup() {

    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }
    startTime = millis();

    // Create tasks
    for(int i=0; i<50; i++) {
        int randomInterval = random(100, 5000);
        char buf[32];
        sprintf(buf,"Rand_%i",randomInterval);
        Task* t = new Task(emptyTask, randomInterval,true,0,buf); 
//                      Task(turnLedOn, 1000,true,20,"OnOff",true);

        scheduler.addTask(t);   
    } 
    scheduler.addTask(&report);   
    scheduler.begin();
    runTimer.start();
}

void loop() {

    runTimer.update();
    if(Serial.available()) {
        String teststr = Serial.readString();
        Serial.printf("%ld Found %s\n",__LINE__,teststr.c_str());
        if(teststr.startsWith("help")) {
            Serial.println("Options: status\n         show all\n         show enabled\n         help\n         ls");
        } else if(teststr.startsWith("status")) {
            String str = scheduler.displayStatus(true);
            Serial.print(str);
            Serial.printf("Loop count is %ld\n",loopRun);
        } else {
            Serial.println("Options: status\n         show all\n         show enabled\n         help\n         ls");
        }
    }

    if(millis() - startTime < 120000) {
        elapsedTime = getTime(millis() - startTime);  
    }
    elapsedMs = millis() - startTime;
}

void runLoop() {
    loopRun++;
    scheduler.run();
}

