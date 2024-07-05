#include "TaskSched.h"

/**
 * @brief Check if this is the first iteration of the task.
 * @return bool True if it's the first iteration, false otherwise.
 */
bool Task::isFirstIteration() {
    return mIterationCount == 0;
}

/**
 * @brief Get the name of the task.
 * @return String The name of the task.
 */
String Task::getName() {
    return mName;
}

/**
 * @brief Check if this is the last iteration of the task.
 * @return bool True if it's the last iteration, false otherwise.
 */
bool Task::isLastIteration() {
    if (mIterations == 0) {
        return false;
    }
    return mIterationCount >= mIterations;
}

/**
 * @brief Disable the task.
 */
void Task::disable() {
    mEnabled = false;
    mIterationCount = 0;
}

// ... (other method implementations)

/**
 * @brief Run the task if conditions are met.
 * 
 * This method checks if the task should be run based on its interval
 * and other conditions, and executes the task's callback if so.
 */
void Task::runIt() {
    if (!isEnabled()) {
        return;
    }

    if ((mRunImmediately && mIterationCount == 0) || (millis() - mLastStartTime) > mInterval) {
        mRunImmediately = false;

        unsigned long diff = millis() - getLastStartTime();
        String sDiff = formatMS(diff);
        String sInt = formatMS(getInterval());
        #ifdef DEBUG
        Serial.printf("%s %d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",
                      __FILE__, __LINE__, getName().c_str(), isEnabled(),
                      sDiff.c_str(), sInt.c_str(), getRunImmediately());
        #endif

        if (mProcWithTask) {
            mProcWithTask(this);
        }

        ++mIterationCount;
        #ifdef DEBUG
        Serial.printf("%s %d %s in runIt iteration count is %ld, max is %ld\n",
                      __FILE__, __LINE__, getName().c_str(), mIterationCount, mIterations);
        #endif

        if (mIterationCount > mIterations && mIterations != 0) {
            #ifdef DEBUG
            Serial.printf("Iterations exhausted for %s, disabling\n", getName().c_str());
            #endif
            disable();
        } else {
            mLastStartTime = millis();
        }
    }
}

// ... (Sched method implementations)

/**
 * @brief Run the scheduler.
 * 
 * This method iterates through all tasks and runs them if they are enabled.
 */
void Sched::run() {
    if (this->mSchedEnabled) {
        for (auto it = tTasks.begin(); it != tTasks.end(); ++it) {
            SafePtr<Task>& currentTask = *it;
            if (currentTask->isEnabled()) {
                currentTask->runIt(); 
            }
        }
    } else {
        #ifdef DEBUG
        Serial.println("Not enabled");
        #endif
    }
}

// Explicit instantiations for common types
template Task::Task<float>(Task::TaskCallback, float, bool, int, const char*, bool);
template Task::Task<double>(Task::TaskCallback, double, bool, int, const char*, bool);
template Task::Task<int>(Task::TaskCallback, int, bool, int, const char*, bool);
template Task::Task<long>(Task::TaskCallback, long, bool, int, const char*, bool);
template Task::Task<unsigned long>(Task::TaskCallback, unsigned long, bool, int, const char*, bool);
