#ifndef TASKSCHED_TPP
#define TASKSCHED_TPP

/**
 * @brief Initialize a task with the given parameters.
 * 
 * This function handles the initialization of a task, including the conversion
 * of the interval to milliseconds if it's a floating-point value.
 * 
 * @tparam T The type of the interval parameter (integral or floating-point)
 * @param interval The time between task executions
 * @param enabled Whether the task should start enabled
 * @param iterations The number of times to run the task (0 for infinite)
 * @param name The name of the task
 * @param runImmediately Whether to run the task immediately upon enabling
 */
template<typename T>
void Task::initializeTask(T interval, bool enabled, int iterations, const char* name, bool runImmediately) {
    if constexpr (std::is_floating_point<T>::value) {
        mInterval = static_cast<long>(interval * 1000.0 + 0.5); // Convert seconds to milliseconds and round
    } else {
        mInterval = static_cast<long>(interval); // Already in milliseconds
    }
    mEnabled = enabled;
    mIterations = iterations;
    mName = name;
    mRunImmediately = runImmediately;
    mOrig.mEnabled = enabled;
    mOrig.mInterval = mInterval;
    mOrig.mIterations = iterations;
    mOrig.mRunImmediately = runImmediately;
    mLastStartTime = millis();

    #ifdef DEBUG
    Serial.print("Interval set to: ");
    Serial.print(mInterval);
    Serial.println(" ms");
    #endif
}

#endif // TASKSCHED_TPP
