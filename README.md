# Overview

Yet another cooperative multitasking scheduler for Arduino processors. I was unable to make the
current crop of schedulers work correctly, so I decided to write my own.
This version is a simpler implementation of the flavor of TaskScheduler
(<https://github.com/arkhipenko/TaskScheduler>). 

TaskSched has been tested on ESP32 and ESP8266 processors. I see no reason that it shouldn't work
on other boards, unless it takes too much memory. I'll eventually test it on some Arduino and Teensy boards.

## Main Features  

1. Periodic task execution. The interval is specified in the task
   creation but can be changed. The times are in milliseconds only.
   Unlike TaskScheduler, the timing isn't necessarily exact if the
   program takes too much time in a task. It's scheduled on a best
   effort basis. If the code is tight enough it'll work if your time
   constraints aren't critical. There is no "catchup" support.
   
2. Support for task enable/disable.   

3. Support for number of iterations the task can make. The task will be
   disabled when the iteration count is exhausted. Unlimited iterations
   are also supported.
   
4. Task can be scheduled to run immediately when enabled if necessary
   or wait for the interval to expire.
   
5. A task can be restarted as it was defined or restarted with new
   parameters, such as different interval, different callback function,
   etc.
   
6. Timing control of the scheduler can be governed by a Ticker  
   (TickTwo) class object or by a call to the Scheduler running in the  
   loop.
   
7. Unlike TaskScheduler the processor does not sleep during idle time.   

## Installation

***
git clone https://github.com/AverageGuy/tasksched.git 
***

## Usage  

### Task Creation  

- *Task(const voidFuncType &callBack, unsigned long interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false)*  

    - *callBack* is a function that will be called when the task has been enabled, the iteration count is still greater than zero and the interval has expired.  
    
    - *interval* is an unsigned long integer specifying the number of milliseconds between executions passes.  
    
    - *enabled* is a flag indicating the task should be placed in the run queue, if set to true.  
    
    - *iterations* is a long integer asking that the task should be run for only the passed value of iterations. If the value is zero then task should be run without a limit of iterations.  
    
    - *name* is a String value of a name of the task.  
    
    - *runImmediate* is a boolean flag that directs the scheduler to run the callback immediately rather than wait for the interval to expire. False says to wait for the interval to run the task.
    
- *Task(const voidFuncTypeWith &callBack, unsigned long interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false)*

    - *callBack* is a function that will be called when the task has been enabled, the iteration count is still greater than zero and the interval has expired. It will be passed a reference to this Task object as the first parameter.  
    
    - *interval*, *enabled*, *iterations*, *name*, and *runImmediate* same as above.
    
There are two variations on the basic constructor, one that passes a reference to the object as the first parameter of the callback function and one that doesn't.  

Note there are 6 other similar constructors that allow you to pass the interval  
as one of *int*, *long*, or *double*. E.g.

- *Task(const voidFuncType &callBack, int interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false)*

### Task Public Methods   

- *bool isFirstIteration()*  

    - Returns true if this is the first iteration
    
- *bool isLastIteration()*

    - Returns true if this is the last iteration
    
- *bool fRunImmediate(bool)*

    - If passed a true value then the task will be run immediately as soon as the scheduler processes the task. It is intended to be run after a restart function call.
    
- *void restart()*  

    - Calling restart uses the values saved at task instantiation, but leaves the task disabled. If you wish to schedule the task you must call enable after the restart. If you need to change parameters such as iterations and interval, use the functions provided. The original saved parameters are not modified.
    
- *void enable()*

    - Enable the task. It will be run during the next iteration of the scheduler or after the interval is completed if fRunImmediate was not called with a true value.
    
- *void disable()*

    - Disable the task. If you disable a task while running and then enable it later, the iteration count is retained. That is, if the iteration count was originally 10 and it has completed 6 iterations when disable was called, then if you enable it at a later time, the task will complete 4 more iterations.
    
- *bool isEnabled()*

    - Test to see if the task is enabled. Returns true if enabled.
    
- *void setCallback(const voidFuncType &)*  

    - Set a new function for the callback. This is best used after a reset but will work on an enabled task. The next iteration of the task will use the new callback.

- *void runIt()*

    - This is the function the scheduler uses to schedule the task, if needed. It shouldn't be called by the user's program.
    
- *String getName()*

    - Return the name of the task in a String.        
    
- *void showInit()*
    
    - Prints the task name, interval, enabled flag, iterations. Probably only good for debugging. The name implies it would show the initial settings but it is actually showing the 
    current settings.
    
- *String formatMS(unsigned long milliseconds)*

    - Returns a string with the time expressed in millisconds as MM:SS.ms e.g. 5:59.124  
    
-   *void setInterval(unsigned long newInterval)*
    
    - Sets the interval for this task to a new value.  It takes effect when ever the scheduler runs again.
    
-   *void setIterations(unsigned long newIterations)*

    - Sets a new iteration count for this task.  It takes effect when ever the scheduler runs again.  If you use
    this function to set iterations for a task that had exhausted its iteration count you need to
    enable the task again.


### Sched Creation  

- *Sched()*  

### Sched Public Methods  

- *unsigned long getSize()*  

    - Returns the length of the list of tasks, or how many tasks are being managed
    
- *String displayStatus(bool all=0, String taskName="")*   

    - Returns a String variable with statistics about scheduled tasks.  If the first parameter is true then  
    the method returns all of the tasks in the schedule's list.  If false then the method looks for a  
    task named in the second parameter.  If not found it returns an empty string.  
    
    - Be very careful using this method if you have a large number of tasks or you are running this on  
    a device with limited memory.  It utilizes a fixed length buffer of 1000 characters to collect the  
    report.  Just be warned.  It may not display all of the tasks since it senses a potential overrun  
    of the 1000 character buffer.  
      
- *void begin()*  

    - This method must be called for the scheduler to run.  It's usually run from a setup function.  
    
- *void addTask(Task *task)*

    - This is used to add a task to the task list.
    
- *void enable()*

    - Used to enable the scheduler.  Not normally needed unless the scheduler was disabled.
    
- *void disable()*

    - Used to disable the scheduler.  No tasks will be run while the scheduler is disabled.
    
- *int isEnabled()*  

    - Returns false if the scheduler has been disabled, else returns true.
    
- *void run()*

    - This method does the scheduling.  It normally should be run periodically and is commonly placed  
    in the loop function.  
    
- *const list<Task *>& getTasks() const*

    - This method simply returns a pointer to the list of tasks being scheduled.
