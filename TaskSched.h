#ifndef TASKSCHED_H
#define TASKSCHED_H

#define DEBUGA 1
#include "Arduino.h"
//#include <list>
#define TASK_SECOND 1000
#define TASK_MINUTE 60*TASK_SECOND

#include <SimpleList.h>
#include <SafePtr.h>
class Task;


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
Task t1(turnLedOn, 1000,false,20,"On1",false);
Task t2(turnLedOn, 2000,false,20,"On2",false);

Output:
450 Task       On2,  Enabled? 0, Diff 00:15.735, Interval 00:02.000, RI 0
450 Task       On1,  Enabled? 0, Diff 00:15.735, Interval 00:00.100, RI 0
450 Task        On,  Enabled? 1, Diff 00:01.721, Interval 00:01.000, RI 0
@endcode
*/
/**
 * @brief Represents a schedulable task.
 * 
 * The Task class encapsulates a function to be called at regular intervals,
 * along with parameters controlling its execution.
 */
class Task {

    public:
    // Function pointer types
    typedef void (*TaskCallback)(Task*);
    typedef void (*VoidCallback)();
private:
    TaskCallback mProcWithTask;
    VoidCallback mProcVoid;
    bool mWithTaskPtr;

//        voidFuncTypeWith mProc;
        unsigned long mInterval;
        bool mEnabled;
        unsigned long mIterations;
        String mName;
        bool mRunImmediately;
        unsigned long mIterationCount;
        bool doShow=true;
        unsigned long mLastStartTime;
        savedInitial mOrig;
        unsigned long passedInterval=1000;
    public:
    /**
     * @brief Constructs a new Task.
     * 
     * @tparam T Type of the interval (can be integral or floating-point)
     * @param func Function to be called at regular intervals
     * @param interval Time between calls to func (in milliseconds if integral, seconds if floating-point)
     * @param enabled Flag to indicate whether the task should start in an enabled state
     * @param iterations Number of times the task will be executed (0 for infinite)
     * @param name A descriptive name for the task
     * @param runImmediately Whether to run the callback immediately or wait for the interval
     * @return The first form returns a pointer to the task, the second does not.
     */
    Task(TaskCallback func, unsigned long interval = 5000, bool enabled = false,
         int iterations = 0, const char* name = "Unk", bool runImmediately = false);

    // Constructor for callback without Task pointer
    Task(VoidCallback func, unsigned long interval = 5000, bool enabled = false,
         int iterations = 0, const char* name = "Unk", bool runImmediately = false);
    /**
     * @brief Gets a SafePtr to this Task.
     * @return SafePtr<Task> A safe pointer to this Task
     */
    SafePtr<Task> getSafePtr() {
        return SafePtr<Task>(this);
    }
    /** Usage:
     *
     * SaafePtr<Task> createTask( parameters ) {
     *     Task* rawTask = new Task( parameters );
     *     return rawTask->getSaafePtr();
     * }
     */
   
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
    /**
     * @brief Sets a new callback function for the task.
     * @param callback The new callback function
     */
        void setCallback(const TaskCallback &);
        void setCallback(const VoidCallback &);
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
        void addTask(SafePtr<Task> task);
        /** enable the scheduler */
        void enable();
        /** disable the scheduler */
        void disable();
        /** return true if the scheduler is enabled */
        int isEnabled();
        /** returns a list of the tasks */
        //const std::list<Task *>& getTasks() const;
//        const SimpleList<Task *>& getTasks() const;
        const SimpleList<SafePtr<Task>>& getTasks() const;
        /** called perodically to check if a task should be scheduled */
        void run();

    private:
        //SimpleList<Task*> tTasks;
        SimpleList<SafePtr<Task>> tTasks;
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
