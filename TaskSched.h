#ifndef TASKSCHED_H
#define TASKSCHED_H

//#define DEBUGA 1
#include "Arduino.h"
//#include <list>
#define TASK_SECOND 1000
#define TASK_MINUTE 60*TASK_SECOND

#include <SimpleList.h>
class Task;


// was typedef std::function<void(Task *)> voidFuncTypeWith;

/**
 * @brief This class holds the initial state of a task.
 *
 * It's kinda like the DTO in the Java world.
 */
class InitialState {
public:

    /**
    * @brief This member holds the original interval value.
    */
    unsigned long mInterval;
    /**
    * @brief    saved enable flag
    */
    bool mEnabled;
    /**
    * @brief    saved run immediately flag
    * @return 
    */
    bool mRunImmediately;
    /**
    * @brief    saved run iterations count
    * @return 
    */
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
Task t1(turnLedOn, 1000,false,100,"On1",false);
Task t2(turnLedOn, 2000,false,2000,"On2",false);

Output:
450 Task       On2,  Enabled? 0, Diff 00:15.735, Interval 00:02.000, RI 1
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

    private:
        /**
         * @brief        function used by the schedule to run this task, shouldn't be called by user
         */
        void  runIt();
        // make Sched a friend so it can call a private method, runIt
        friend class Sched;

    public:
        // Function pointer types
        typedef void (*TaskCallback)(Task*);
        typedef void (*VoidCallback)();
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

    private:
        /**
         * @brief  Contains a pointer to the function that will be called when the task is run. 
         * @return It returns a pointer to the Task object.
         */
        TaskCallback mProcWithTask;
        /**
         * @brief  Contains a pointer to the function that will be called when the task is run. 
         */
        VoidCallback mProcVoid;
        /**
         * @brief  Contains a pointer to the function that will be called when the task is run. 
         */
        bool mWithTaskPtr;

        /**
         * @brief This stores the interval that was passed at object creation.
         */
        unsigned long mInterval;
        /**
         * @brief  This stores the enable status of the Task.  It was set at object creation.
         */
        bool mEnabled;
        /**
         * @brief This is the number of iterations that the Task object will run.
         */
        unsigned long mIterations;
        /**
         * @brief Name of the Task set at creation.
         */
        String mName;
        /**
         * @brief This is a flag that tells the scheduler to initiall run the Task immediately.  Otherwise it will run after
         *        the interval expires.
         */
        bool mRunImmediately;
        /**
         * @brief This is where the iteration counter is stored.
         */
        unsigned long mIterationCount;
        /**
         * @brief This is a flag used internally to limit the showInit method runs only once.
         */
        bool doShow=true;
        /**
         * @brief 
         */
        unsigned long mLastStartTime;
        /**
         * @brief 
         */
        savedInitial mOrig;
        /**
         * @brief        return true if this is the first iteration
         * @return 
         */
    public:
        bool isFirstIteration();
        /**
         * @brief        return true if this is the last iteration
         * @return 
         */
        bool isLastIteration(); 
        /**
         * @brief        return true if the run immediately flag is set
         * @return 
         */
        bool fRunImmediately();
        /**
         * @brief        restart the task with the original parameters, Enable is not restored
         */
        void restart(); 
        /**
         * @brief        enable the task
         */
        void enable(); 
        /**
         * @brief       disable the task
         */
        void disable(); 
        /**
         * @brief        return true if task is enabled
         * @return 
         */
        bool isEnabled();
        /**
         * @brief Sets a new callback function for the task.
         * @param callback The new callback function
         */
        void setCallback(const TaskCallback &callback);
        /**
         * @brief Sets a new callback function for the task.
         * @param callback The new callback function
         */
        void setCallback(const VoidCallback &callback);
        /**
         * @brief        give the task a new name
         */
        void setName(String);
        /**
         * @brief        display stuff
         */
        void showInit();
        /**
         * @brief        return string containing name of task
         * @return 
         */
        String getName();
        /**
         * @brief        return the iteration count, that is the number of iterations that the task has been run
         * @return 
         */
        unsigned long getIterationCount();
        /**
         * @brief        function to set a new interval
         */
        void setInterval(unsigned long newInterval);
        /**
         * @brief        function to set a new iterations value
         */
        void setIterations(unsigned long newIterations);
        /**
         * @brief        function to set the run immediately flag
         */
        void setImmediately(bool);
        /**
         * @brief        function that displays task info
         *
         * @return String containing info about all tasks tasks.
         */
        String showTaskInfo();
        /**
         * @brief        return the task interval
         * @return 
         */
        unsigned long getInterval(void);
        /**
         * @brief        return the run immediately flag
         * @return 
         */
        bool getRunImmediately(void);
        /**
         * @brief        return the last start time flag
         * @return 
         */
        unsigned long getLastStartTime(void);
        /**
         * @brief        return a string with a formatted time
         * @return 
         */
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

/**
 * @brief This class runs the scheduled tasks.
 *
 * It's kinda like the DTO in the Java world.
 */
class Sched
{
    public:
        /**
        * @brief        return the number of tasks in the run queue
        * @return 
        */
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
        /**
        * @brief 
        * @return 
        */
        String displayStatus(int num,String taskName="",bool raw=false);
        Sched();
      /** used to start the scheduling. A call to begin will also enable the scheduler.
```
Sched scheduler;  
	scheduler.begin();
```
         */
        /**
        * @brief 
        */
        void begin();
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
        /**
        * @brief 
        */
        void addTask(Task *task);
        /**
        * @brief        enable the scheduler
        */
        void enable();
        /**
        * @brief        disable the scheduler
        */
        void disable();
        /**
        * @brief        return true if the scheduler is enabled
        * @return 
        */
        int isEnabled();
        //const std::list<Task *>& getTasks() const;
//        const SimpleList<Task *>& getTasks() const;
        const SimpleList<Task *>& getTasks() const;
        /**
        * @brief        called perodically to check if a task should be scheduled
        */
        void run();

    private:
        //SimpleList<Task*> tTasks;
        /**
        * @brief 
        * @return 
        */
        SimpleList<Task *> tTasks;
        /**
        * @brief 
        * @return 
        */
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
