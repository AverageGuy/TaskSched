#ifndef TASKSCHED_H
#define TASKSCHED_H

#define DEBUGA 1
#include "Arduino.h"
#define TASK_SECOND 1000
#define TASK_MINUTE 60*TASK_SECOND

#include <SimpleList.h>
#include <SafePtr.h>
#include <type_traits>
class Task;

/**
 * @brief Represents the initial state of a Task.
 */
class InitialState {
public:
    unsigned long mInterval;    /**< The initial interval */
    bool mEnabled;              /**< The initial enabled state */
    bool mRunImmediately;       /**< The initial run immediately flag */
    unsigned long mIterations;  /**< The initial number of iterations */
};
using savedInitial = InitialState;

/**
 * @brief Represents a schedulable task.
 * 
 * The Task class encapsulates a function to be called at regular intervals,
 * along with parameters controlling its execution.
 */
class Task {
public:
    /** Function pointer type for task callbacks */
    typedef void (*TaskCallback)(Task*);

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
     */
    template<typename T>
    Task(TaskCallback func, T interval, bool enabled = false,
         int iterations = 0, const char* name = "Unk", bool runImmediately = false);

    /**
     * @brief Sets a new callback function for the task.
     * @param callback The new callback function
     */
    void setCallback(const TaskCallback& callback);

    /**
     * @brief Gets a SafePtr to this Task.
     * @return SafePtr<Task> A safe pointer to this Task
     */
    SafePtr<Task> getSafePtr();

    // ... (other method declarations)

private:
    template<typename T>
    void initializeTask(T interval, bool enabled, int iterations, const char* name, bool runImmediately);

    TaskCallback mProcWithTask;  /**< The task's callback function */
    long mInterval;              /**< The task's interval in milliseconds */
    bool mEnabled;               /**< Whether the task is currently enabled */
    unsigned long mIterations;   /**< Number of iterations for this task */
    String mName;                /**< The name of this task */
    bool mRunImmediately;        /**< Whether to run immediately when enabled */
    unsigned long mIterationCount; /**< Current iteration count */
    bool doShow;                 /**< Whether to show debug information */
    unsigned long mLastStartTime; /**< Last time the task was started */
    savedInitial mOrig;          /**< Original state of the task */
    double passedInterval;       /**< Interval passed to constructor */

public:
    // ... (other public method declarations)
};

/**
 * @brief Scheduler for managing multiple tasks.
 */
class Sched {
public:
    /**
     * @brief Get the number of tasks in the run queue.
     * @return unsigned long The number of tasks
     */
    unsigned long getSize();

    /**
     * @brief Display the status of tasks.
     * @param num Number of tasks to display (0 for all enabled tasks)
     * @param taskName Name of a specific task to display (if empty, displays based on num)
     * @param raw Whether to display raw interval values
     * @return String A string containing the status information
     */
    String displayStatus(int num, String taskName="", bool raw=false);

    /**
     * @brief Default constructor.
     */
    Sched();

    /**
     * @brief Start the scheduling.
     * 
     * This method must be called to enable the scheduler.
     */
    void begin();

    /**
     * @brief Add a task to the run queue.
     * @param task SafePtr to the Task to be added
     */
    void addTask(SafePtr<Task> task);

    /**
     * @brief Enable the scheduler.
     */
    void enable();

    /**
     * @brief Disable the scheduler.
     */
    void disable();

    /**
     * @brief Check if the scheduler is enabled.
     * @return int 1 if enabled, 0 if disabled
     */
    int isEnabled();

    /**
     * @brief Get a reference to the list of tasks.
     * @return const SimpleList<SafePtr<Task>>& Reference to the list of tasks
     */
    const SimpleList<SafePtr<Task>>& getTasks() const;

    /**
     * @brief Run the scheduler.
     * 
     * This method should be called periodically to check and run scheduled tasks.
     */
    void run();

private:
    SimpleList<SafePtr<Task>> tTasks;  /**< List of tasks managed by this scheduler */
    int mSchedEnabled;                 /**< Whether the scheduler is currently enabled */
};

#include "TaskSched.tpp"

#endif // TASKSCHED_H
