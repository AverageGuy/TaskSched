# TaskSched
Cooperative multitasking library.
Overview
========

THIS LIBRART ISN'T READY FOR PRODUCTION.  DON'T BE SURPRISED IF IT DOESN'T WORK OR IF IT CHANGES WITHOUT WARNING.

Yet another cooperative multitasking scheduler. I was unable to make the
current crop of schedulers work correctly, so I decided to write my own.
This version is a simpler implementation of the flavor of TaskScheduler
(<https://github.com/arkhipenko/TaskScheduler>[).]()

Main features:

1.  Periodic task execution. The interval is specified in the task
    creation but can be changed. The times are in milliseconds only.
    Unlike TaskScheduler, the timing isn't necessarily exact if the
    program takes too much time in a task. It's scheduled on a best
    effort basis. If the code is tight enough it'll work if your time
    constraints aren't critical. There is no "catchup" support.

2.  Support for task enable/disable.

3.  Support for number of iterations the task can make. It will be
    disabled when the iteration count is exhausted. Unlimited iterations
    are also supported.

4.  Task can be scheduled to run immediately when enabled if necessary
    or wait for the interval to expire.

5.  A task can be restarted as it was defined or restarted with new
    parameters, such as different interval, different callback function,
    etc.

6.  Timing control of the scheduler can be governed by a Ticker
    (TickTwo) class object or by a call to the Scheduler running in the
    loop.

7.  Unlike TaskScheduler the processor does not sleep during idle time.

Installation 
============

git clone
[https://github.com/AverageGuy/tasksched.git]

Usage
=====

Task creation
-------------

-   Task(const voidFuncType & callBack, unsigned long interval=5000,\
    bool enabled=false,\
    unsigned long iterations=0,\
    String name=\"Unk\",\
    bool runImmediate=false)

-   Task(const voidFuncType & callBack)

    _callBack_ is a function that will be called when the task
    has been enabled, the iteration count is still greater than zero and
    the interval has expired.

    _interval_ is a unsigned long integer specifying the number of
    milliseconds between executions passes.

    _enabled_ is a flag indicating the task should be placed in the run
    queue, if set to true.

    _iterations_ is a long integer asking that the task should be run for
    only the passed value of iterations. If the value is zero then task
    should be run without a limit of iterations.

    _name_ is a String value of a name of the task.

    _runImmeidate_ is a boolean flag that directs the scheduler to run the
    callback immediately rather than wait for the interval to expire.
    False says to wait for the interval to run the task.

-   bool isFirstIteration()

    Returns true if this is the first iteration

-   bool isLastIteration()
  
    Returns true if this is the last itereation

-   bool fRunImmediate(bool)
   
    If thi function is passed a true value then the task will be run immediately as soon
    as the scheduler processes the task. It is intended to be run after
    a restart function call.

-   void restart();

    Calling restart uses the values saved at task instantiation, but
    leaves the task disabled. If you wish to schedule the task you must
    call enable after the restart. If you need to change parameters such
    as iterations and interval, use the functions provided. The original
    saved parameters are not modified.

-   void enable();

    Enable the task. It will be run during the next iteration of the
    scheduler or after the interval is completed if fRunImmediate was
    not called with a true value.

-   void disable();

    Disable the task. If you disable a task while running and then
    enable it later, the iteration count is retained. That is, if the
    iteration count was originally 10 and it has completed 6 iterations
    when disable was called, then if you enable it at a later time, the
    task will complete 4 more iterations.

-   bool isEnabled();

    Test to see if the task is enabled. Returns true if enabled.

-   void setCallback(const voidFuncType &);

    Set a new function for the callback. This is best used after a reset
    but will work on an enabled task. The next iteration of the task
    will use the new callback.

-   void runIt();

    This is the function the scheduler uses to schedule the task, if
    needed. It shouldn't be called by the user's program.

-   String getName();

    Return the name of the task in a String.  Probaby only useful for debugging.

-   void setInterval(unsigned long newInterval)

    Set a new interval.  If you use this call, followed by a restart, it will not have any effect since restart resets the interval to the original value.

-   void setIterations(unsigned long newIterations)

    Set a new iteration count. If you use this call, followed by a restart, it will not have any effect since restart resets the iteration count to the original value.

    The effect of the last two calls is unpredictable so it's advisable to disable the task before executing either and then enable it again.
 
