#include "TaskSched.h"
Task::Task(const voidFuncType & func,  unsigned long interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediate)
        : mProc(func){
        mRunImmediate=runImmediate;
        mInterval=interval;
        mEnabled=enabled;
        mIterations=iterations;
        mIterationCount=0;
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediate=runImmediate;
        mLastStartTime=millis();
        mName=name;
#ifdef DEBUG
        Serial.printf("Stats for %s, interval = %ld, enabled = %d, iterations = %ls\n",name,interval,iterations);
#endif
        };

Task::Task(const voidFuncType & func,  double interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediate)
        : mProc(func){
        mRunImmediate=runImmediate;
        mInterval=interval*TASK_SECOND;
        mEnabled=enabled;
        mIterations=iterations;
        mIterationCount=0;
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediate=runImmediate;
        mLastStartTime=millis();
        mName=name;
#ifdef DEBUG
        Serial.printf("Stats for %s, interval = %ld, enabled = %d, iterations = %ls\n",name,interval,iterations);
#endif
        //mProc=func;
        };
//    Task(*functionPtr)(),mInterval=5000,mEnabled=false,mIterations=0);


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

String Task::showTaskInfo() {

    char buf[100];
    sprintf(buf,"Enabled: %d, Immediate: %d, Interval: %ld, Iter: %ld, IterCnt: $ld, Name: %s\n",mEnabled, mRunImmediate, mInterval, mIterations, mIterationCount);
    String ret(buf);
    return ret;
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

unsigned long Task::getRunImmediate(void)
{
    return mRunImmediate;
}

unsigned long Task::getInterval(void)
{
    return mInterval;
}

unsigned long Task::getLastStartTime(void)
{
    return mLastStartTime;
}

void  Task::runIt() {
#ifdef DEBUG
//    Serial.printf("%s called for runIt\n",mName.c_str());
#endif
    if(mRunImmediate && mIterationCount == 0) {
        mProc();
        ++(mIterationCount);
        return;
    }
    if(millis() - mLastStartTime > mInterval){
        mProc();
        ++(mIterationCount);
#ifdef DEBUG
//        Serial.printf("%s in runit iteration count is %ld, max is %ld\n",getName(),mIterationCount,mIterations);
#endif
        if(mIterationCount>mIterations && mIterations != 0) {
#ifdef DEBUG
//            Serial.printf("Iterations exhaused for %s, diabling\n",getName());
#endif
            disable();
        } else {
            mLastStartTime=millis();
        }
#ifdef DEBUG
//        Serial.printf("%s in runit count is %ld\n",getName(),mIterationCount);
#endif
    }
}

