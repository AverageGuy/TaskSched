#include "TaskSched.h"
Task::Task(const voidFuncType & func,   long interval,
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
Task::Task(const voidFuncType & func,  int interval,
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
Task::Task(const voidFuncType & func,  unsigned long interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediate)
    : mProc(func){
        mRunImmediate=runImmediate;
        mInterval=interval;
        Serial.printf("For long interval passed in %ld, mInterval became %ld\n",interval,mInterval);
        mEnabled=enabled;
        mIterations=iterations;
        mIterationCount=0;
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediate=runImmediate;
        mLastStartTime=millis();
        mName=name;
        Serial.printf("%s instantiated\n",name.c_str());
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
        mInterval=(unsigned long)interval;
        passedInterval = interval;
        mEnabled=enabled;
        mIterations=iterations;
        mIterationCount=0;
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediate=runImmediate;
        mLastStartTime=millis();
        mName=name;
        Serial.printf("%s instantiated\n",name.c_str());
        //Serial.println("Befoe call");
        //mProc();
        //Serial.println("After call");
#ifdef DEBUG
        Serial.printf("Stats for %s, interval = %ld, enabled = %d, iterations = %ls\n",name,interval,iterations);
#endif
        //mProc=func;
    };
//    Task(*functionPtr)(),mInterval=5000,mEnabled=false,mIterations=0);


bool Task::isFirstIteration() {
    if(mIterationCount == 0) {
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

    String res;
    res = formatMS(mInterval);
    //    Serial.printf("Name: %s, interval: %ld, enabled:%d, iterations:%ld\n", mName,mInterval,mEnabled,mIterations);
    Serial.printf("Name: %s, interval: %s, enabled:%d, iterations:%ld\n",
            mName,res,mEnabled,mIterations);
#endif
    return;
}

String Task::formatMS(unsigned long milliseconds) 
{
    char res[32];
    int minutes = milliseconds / 60000;
    int seconds = (milliseconds % 60000) / 1000;
    int millisecondsRemaining = (milliseconds % 60000) % 1000;
    sprintf(res,"%d:%02d:%03d",minutes, seconds, millisecondsRemaining);
    Serial.print(__LINE__);
    Serial.print(" ");
    Serial.print(res);
    Serial.print(" ");
    String result(res);
    Serial.println(result);
    return result;
}

void Task::showTaskInfo() {

    Task *currentTask = this;
    unsigned long diff = millis() - currentTask->getLastStartTime();
    String sDiff= currentTask->formatMS(diff);
    String sInt= currentTask->formatMS(currentTask->getInterval());
    //   Serial.printf("-- interval %ld %s %s\n",currentTask->getInterval(),sInt,sInt.c_str());
    Serial.printf("%ld Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__LINE__,currentTask->getName(),currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediate());
    //    sprintf(buf,"RunIt called %ld, Enabled: %d, Immediate: %d, Interval: %ld, Iter: %ld, IterCnt: %ld, Name: %s\n",mRunItCalled,mEnabled, mRunImmediate, mInterval, mIterations, mIterationCount,mName);
    //    Serial.println(buf);
    if(passedInterval >0) {
        Serial.printf("For float interval passed in %f, mInterval became %ld\n",passedInterval,mInterval);
    }
    return;
}

bool Task::isEnabled() {
    return mEnabled;
}

void Task::enable() {
    mEnabled=true;
    mLastStartTime=millis();
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

void Task::setCallback(const voidFuncType & func) {
    mProc=func;
}

void Task::setImmediate(bool newImmediate)
{
    mRunImmediate = newImmediate;
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
    //Serial.print(showTaskInfo());
    if(!isEnabled()) {
        return;
    }
    mRunItCalled++;
    /*
       if(mRunImmediate && mIterationCount == 0) {
       Serial.printf("%s Run proc immediate\n",mName);
    // best way to do that is to fake the time
    // by reducing mLastStartTime to zero
    // That way the normal run will be made
    mLastStartTime=0L;
    mRunImmediate=false; // don't do it again
    }
    */
    if((mRunImmediate && mIterationCount == 0) || (millis() - mLastStartTime > mInterval)){
        Serial.printf("%s Run proc now\n",mName);
        mRunImmediate=false; // don't do it again
        unsigned long xdiff = millis() - mLastStartTime;
                Task *currentTask = this;
                unsigned long diff = millis() - currentTask->getLastStartTime();
                String sDiff= currentTask->formatMS(diff);
                String sInt= currentTask->formatMS(currentTask->getInterval());
             //   Serial.printf("-- interval %ld %s %s\n",currentTask->getInterval(),sInt,sInt.c_str());
                Serial.printf("%ld Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__LINE__,currentTask->getName(),currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediate());
        //Serial.printf("%s - millis %ld, mLastStartTime %ld, mInterval %ld, diff %ld\n",getName(),millis(),mLastStartTime,mInterval,diff);
        mProc();
        ++(mIterationCount);
#ifdef DEBUG
        Serial.printf("%s in runit iteration count is %ld, max is %ld\n",getName(),mIterationCount,mIterations);
#endif
        if(mIterationCount>=mIterations && mIterations != 0) {
#ifdef DEBUG
            Serial.printf("Iterations exhaused for %s, diabling\n",getName());
#endif
            Serial.printf("Iterations exhaused for %s, diabling\n",getName());
            disable();
        } else {
            mLastStartTime=millis();
        }
#ifdef DEBUG
        //        Serial.printf("%s in runit count is %ld\n",getName(),mIterationCount);
#endif
    }
}

