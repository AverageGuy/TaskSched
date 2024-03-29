#ifndef TASKSCHED_H
#define TASKSCHED_H

#define DEBUGA 1
#include "Arduino.h"
#include <list>
#include <functional>
#define TASK_SECOND 1000
#define TASK_MINUTE 60*TASK_SECOND

class Task;
using voidFuncTypeWith = std::function<void(Task *)>;
// was typedef std::function<void(Task *)> voidFuncTypeWith;

class InitialState {
public:

    unsigned long mInterval;
    /** saved enable flag */
    bool mEnabled;
    /** saved run immediately flag */
    bool mRunImmediately;
    /** saved run iterations count */
    unsigned long mIterations;
};
using savedInitial = InitialState;

/** Define a new Task.
 * @param func:
 * Function to be called at regular intervals.
 * @param interval:
 * Time between calls to func, specified in milliseconds or seconds.
 * If interval is a floating-point value, it's interpreted as seconds; otherwise, it's treated as milliseconds.
 * @param enabled:
 * Flag to indicate whether the task should start in an enabled state (true) or disabled state (false).
 * @param iterations:
 * Number of times the task will be executed. Set to 0 for infinite iterations.
 * @param name:
 * A descriptive string name for the task.
 * @param runImmediately:
 * Boolean flag determining whether to run the callback immediately (true) or wait for the interval to expire (false).
 * 
 */

/**
@code 
Task* t = new Task(turnLedOn, 1000,true,20,"OnOff",true);
Task t1(turnLedOn, 100,false,20,"On1",false);
Task t2(turnLedOn, 2.0,false,20,"On2",false);

Output:
450 Task       On2,  Enabled? 0, Diff 00:15.735, Interval 00:02.000, RI 0
450 Task       On1,  Enabled? 0, Diff 00:15.735, Interval 00:00.100, RI 0
450 Task        On,  Enabled? 1, Diff 00:01.721, Interval 00:01.000, RI 0
@endcode
*/
class Task {
    private:
        voidFuncTypeWith mProc;
        long mInterval;
        bool mEnabled;
        unsigned long mIterations;
        String mName;
        bool mRunImmediately;
        unsigned long mIterationCount;
        bool doShow=true;
        unsigned long mLastStartTime;
        savedInitial mOrig;
        double passedInterval=-1.0;
    public:
        Task(void (*func)(Task*) , double interval = 5.0, bool enabled = false,  
                int iterations = 0, String name = "Unk", bool runImmediately = false) 
        {
            mProc=func;
            mIntI=static_cast<int>(interval);
            mInterval=static_cast<int>(interval*1000);
            mEnabled=enabled; 
            mIterations=iterations; 
            mName=name; 
            mRunImmediately=runImmediately;
            mOrig.mEnabled=enabled;
            mOrig.mInterval=mInterval;
            mOrig.mIterations=iterations;
            mOrig.mRunImmediately=runImmediately;
            mLastStartTime=millis();
        }
     //   Task(const Task& func
        Task(void (*func)(Task*) , int interval = 5000, bool enabled = false,  
                int iterations = 0, String name = "Unk", bool runImmediately = false) 
        {
            mProc=func;
            mIntI=interval;
            mInterval=interval;
            mEnabled=enabled; 
            mIterations=iterations; 
            mName=name; 
            mRunImmediately=runImmediately;
            mOrig.mEnabled=enabled;
            mOrig.mInterval=interval;
            mOrig.mIterations=iterations;
            mOrig.mRunImmediately=runImmediately;
            mLastStartTime=millis();
        };
    public:
        int mIntI;
        /** return true if this is the first iteration */
        bool isFirstIteration();
        /** return true if this is the last iteration */
        bool isLastIteration(); 
        /** return true if the run immediately flag is set */
        bool fRunImmediately();
        /** restart the task with the original parameters, Enable is not restored */
        void restart(); 
        /** enable the task */
        void enable(); 
        /**disable the task */
        void disable(); 
        /** return true if task is enabled */
        bool isEnabled();
        /** assign a new callback */
        void setCallback(const voidFuncTypeWith &);
        /** give the task a new name */
        void setName(String);
        /** function used by the schedule to run this task, shouldn't be called by user */
        void  runIt();
        /** display stuff */
        void showInit();
        /** return string containing name of task */
        String getName();
        /** return the iteration count, that is the number of iterations that the task has been run */
        unsigned long getIterationCount();
        /** function to set a new interval */
        void setInterval(unsigned long newInterval);
        /** function to set a new iterations value */
        void setIterations(unsigned long newIterations);
        /** function to set the run immediately flag */
        void setImmediately(bool);
        /** function that displays task info */
        String showTaskInfo();
        /** return the task interval */
        unsigned long getInterval(void);
        /** return the run immediately flag */
        bool getRunImmediately(void);
        /** return the last start time flag */
        unsigned long getLastStartTime(void);
        /** return a string with a formatted time */
        String formatMS(unsigned long milliseconds);
};
/**
@code 
Task* t = new Task(turnLedOn, 1000,true,20,"OnOff",true);
Task t1(turnLedOn, 100,false,20,"On1",false);
Task t2(turnLedOn, 2.0,false,20,"On2",false);

Output as generated by scheduler.displayStatus(true):

450 Task       On2,  Enabled? 0, Diff 00:15.735, Interval 00:02.000, RI 0
450 Task       On1,  Enabled? 0, Diff 00:15.735, Interval 00:00.100, RI 0
450 Task        On,  Enabled? 1, Diff 00:01.721, Interval 00:01.000, RI 0
@endcode
*/

class Sched
{
    public:
        /** return the number of tasks in the run queue */
        unsigned long getSize();
        /** function to display the status of the tasks 
```
Sample output:
	Task       On2,  Enabled? 0, Diff 59:40.731, Interval 00:02.000, RI 0
	Task       On1,  Enabled? 0, Diff 59:40.731, Interval 00:00.100, RI 0
	Task        On,  Enabled? 0, Diff 59:19.710, Interval 00:01.000, RI 0
Source:
    String str = scheduler.displayStatus(true);
    Serial.print(str);
```
         * */
        String displayStatus(int num,String taskName="",bool raw=false);
        /** default constructor */
        Sched();
        /** used to start the scheduling. A call to begin will also enable the scheduler. 
```
Sched scheduler;  
	scheduler.begin();
```
         */
        void begin();
        /** add a task to the run queue */
/**
```
Example: 

Task* t = new Task(turnLedOn, 1000,true,20,"OnOff",true);
Task t1(dummy, 100, false, 20, "On1", * false);
Task t2(dummy, 2.0, false, 20, "On2", * false);

    scheduler.addTask(t);
    scheduler.addTask(&t1);
    scheduler.addTask(&t2);
```
*/
        void addTask(Task* task);
        /** enable the scheduler */
        void enable();
        /** disable the scheduler */
        void disable();
        /** return true if the scheduler is enabled */
        int isEnabled();
        /** returns a list of the tasks */
        const std::list<Task *>& getTasks() const;
        /** called perodically to check if a task should be scheduled */
        void run();

    private:
        std::list<Task *> tTasks;
        int mSchedEnabled;
};

///home/jwl/sketchbook/libraries/TaskSched/TaskSched.h 127 Task Rand_4720, Diff 0:09:054, Interval 0:04:720, RI 0
 /* @param func    Function to be called.
 * @param interval      Time between calls to func in milliseconds.
 * If intval is passed as a floating point number, e. g. 4.5 then it is treated as if it were a number of seconds otherwise the interval is treated as if it were a number of millisconds
 * @param enabled   Flag to be set if the task should start in an enabled state.
 * @param iterations    Number of iterations the task will be run.  Set to zero for infinited iterations.
 * @param name   A String value of a name of the task.
 * @param runImmediately  A boolean flag that directs the scheduler to run the callback immediately rather than wait for the interval to expire. False says to wait for the interval to run the task.
*/
#endif
