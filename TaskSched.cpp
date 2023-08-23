#include "TaskSched.h"

bool Task::isFirstIteration() {
    if(mIterationCount == 1) {
        return true;
    }
    return false;
}

String Task::getName()
{
    return mName;
}

bool Task::isLastIteration() {
    if(mIterations ==0) {
        return false;
    }
    if(mIterationCount >=mIterations) {
        return true;
    }
    return false;
}

void Task::disable() {
    mEnabled=false;
    mIterationCount=0;
    return;
}

void Task::showInit() {
    if(!doShow) {
        return;
    }
    doShow=false; // only do it once
#ifdef DEBUG

    Serial.printf("Name: %s, interval: %ld, enabled:%d, iterations:%ld\n",
        mName,mInterval,mEnabled,mIterations);
#endif
    return;
}

bool Task::isEnabled() {
    return mEnabled;
}

void Task::enable() {
    mEnabled=true;
#ifdef DEBUG
    Serial.printf("Enabling %s\n",mName.c_str());
#endif
    return;
}

void Task::restart() {
    mInterval=mOrig.mInterval;
    mIterations=mOrig.mIterations;
    mEnabled=false;
    mIterationCount=0;
    mRunImmediate=mOrig.mRunImmediate;
#ifdef DEBUG
    Serial.printf("Restarting %s\n",mName.c_str());
#endif
    return;
}

void Task::setIterations(unsigned long newIterations)
{
    mIterations = newIterations;
}

void Task::setInterval(unsigned long newInterval)
{
    mInterval = newInterval;
}

void  Task::runIt() {
#ifdef DEBUG
//    Serial.printf("%s called for runIt\n",mName.c_str());
#endif
    if(mRunImmediate && mIterationCount == 0) {
        mProc();
        ++(this->mIterationCount);
        return;
    }
    if(millis() - mLastStartTime > mInterval){
        mProc();
        ++(this->mIterationCount);
#ifdef DEBUG
//        Serial.printf("%s in runit iteration count is %ld, max is %ld\n",this->getName(),mIterationCount,mIterations);
#endif
        if(mIterationCount>mIterations && mIterations != 0) {
#ifdef DEBUG
//            Serial.printf("Iterations exhaused for %s, diabling\n",this->getName());
#endif
            this->disable();
        } else {
            mLastStartTime=millis();
        }
#ifdef DEBUG
//        Serial.printf("%s in runit count is %ld\n",this->getName(),mIterationCount);
#endif
    }
}

