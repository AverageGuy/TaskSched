#ifndef TASKSCHED_TPP
#define TASKSCHED_TPP

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
    mOrig.mEnabled=enabled;
    mOrig.mInterval=mInterval;
    mOrig.mIterations=iterations;
    mOrig.mRunImmediately=runImmediately;
    mLastStartTime=millis();

    // ... other initializations ...

    #ifdef DEBUG
    Serial.print("Interval set to: ");
    Serial.print(mInterval);
    Serial.println(" ms");
    #endif
}
#endif // TASKSCHED_TPP
