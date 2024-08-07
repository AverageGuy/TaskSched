//#define DEBUG 1
#include "TaskSched.h"
#ifdef DEBUG
//#include "esp_debug_helpers.h"
#endif

Task::Task(TaskCallback func, unsigned long interval, bool enabled, int iterations, const char* name, bool runImmediately)
    : mProcWithTask(func), mProcVoid(nullptr), mWithTaskPtr(true)
{
    // Initialize other members...
            //mProc=func;
            mInterval=interval;
            mEnabled=enabled; 
            mIterations=iterations; 
            mName=name; 
            mRunImmediately=runImmediately;
            mOrig.mEnabled=enabled;
            mOrig.mInterval=mInterval;
            mOrig.mIterations=iterations;
            mOrig.mRunImmediately=runImmediately;
            mLastStartTime=millis();
            mIterationCount=0;
}

Task::Task(VoidCallback func, unsigned long interval, bool enabled, int iterations, const char* name, bool runImmediately)
    : mProcWithTask(nullptr), mProcVoid(func), mWithTaskPtr(false)
{
    // Initialize other members...
            //mProc=func;
            mInterval=interval;
            mEnabled=enabled; 
            mIterations=iterations; 
            mName=name; 
            mRunImmediately=runImmediately;
            mOrig.mEnabled=enabled;
            mOrig.mInterval=interval;
            mOrig.mIterations=iterations;
            mOrig.mRunImmediately=runImmediately;
            mLastStartTime=millis();
            mIterationCount=0;
}

bool Task::isFirstIteration() {
    if(mIterationCount == 0) {
        return true;
    }
    return false;
}

String Task::getName() const
{
    return mName;
}

bool Task::isLastIteration() {
//    Serial.printf("count = %ld, Iter = %ld\n",mIterationCount,mIterations);
    if(mIterations <= 0) {
        return false;
    }
    if(mIterationCount >=mIterations-1) {
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

    res = Task::formatMS(mInterval);
    Serial.printf("%s %d showInit: Name: %s, interval: %s, enabled:%d, iterations:%ld %x\n",
            __FILE__,__LINE__,mName,res,mEnabled,mIterations,this);
#endif

    return;
}

String Task::formatMS(unsigned long milliseconds){

    int minutes = milliseconds / 60000;
    int seconds = (milliseconds % 60000) / 1000; 
    int ms = (milliseconds % 1000);

    char res[20];
    sprintf(res, "%02d:%02d.%03d", minutes, seconds, ms);
#ifdef DEBUG
    Serial.printf("%s %d %s\n",__FILE__,__LINE__,res);
#endif

    return String(res); 
}

String Task::showTaskInfo()  const
{

    char buf[200];
    unsigned long diff = millis() - getLastStartTime();
    String sDiff= Task::formatMS(diff);
    String sInt= Task::formatMS(getInterval());
#ifdef DEBUG
    Serial.printf("%s %d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__FILE__,__LINE__,this->getName().c_str(),isEnabled(),sDiff.c_str(),sInt.c_str(),getRunImmediately());
    if(passedInterval >0) {
        Serial.printf("For float interval passed in %f, mInterval became %ld\n",passedInterval,mInterval);
    }
#else
    sprintf(buf,"Task %s, Enabled? %d, Diff %s, Interval %s, RunIm %d\n",this->getName().c_str(),isEnabled(),sDiff.c_str(),sInt.c_str(),getRunImmediately());
    String ret(buf);
    return ret;

#endif

    return "";
}

bool Task::isEnabled() const {
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
    mRunImmediately=mOrig.mRunImmediately;
#ifdef DEBUG
    Serial.printf("Restarting %s\n",mName.c_str());
#endif
    return;
}

void Task::setCallback(const VoidCallback & func) {
    //mProc=func;
    mProcVoid=func;
}

void Task::setCallback(const TaskCallback & func) {
    //mProc=func;
    mProcWithTask=func;
}

void Task::setName(String newName) {
    mName=newName;
}

void Task::setImmediately(bool newImmediately)
{
    mRunImmediately = newImmediately;
}

void Task::setIterations(unsigned long newIterations)
{
    mIterations = newIterations;
}

void Task::setInterval(unsigned long newInterval)
{
    mInterval = newInterval;
}

bool Task::getRunImmediately(void) const
{
    return mRunImmediately;
}

unsigned long Task::getInterval(void) const
{
    return mInterval;
}

unsigned long Task::getLastStartTime(void) const
{
    return mLastStartTime;
}

unsigned long Task::getIterationCount() const 
{
    return mIterationCount;
}
void Task::runIt() {
    if (!isEnabled()) {
        return;
    }

    if ((mRunImmediately && mIterationCount == 0) || (millis() - mLastStartTime) > mInterval) {
        mRunImmediately = false;

        unsigned long diff = millis() - getLastStartTime();
        String sDiff= Task::formatMS(diff);
        String sInt= Task::formatMS(getInterval());
#ifdef DEBUG
        Serial.printf("%s %d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__FILE__,__LINE__,getName().c_str(),isEnabled(),sDiff.c_str(),sInt.c_str(),getRunImmediately());
#endif
        if (mWithTaskPtr && mProcWithTask) {
            mProcWithTask(this);
        } else if (!mWithTaskPtr && mProcVoid) {
            mProcVoid();
        }

        ++(mIterationCount);
#ifdef DEBUG
        Serial.printf("%s %d %s in runit iteration count is %ld, max is %ld\n",__FILE__,__LINE__,getName().c_str(),mIterationCount,mIterations);
#endif
        if(mIterationCount>=mIterations && mIterations != 0) {
            disable();
        } else {
            mLastStartTime=millis();
        }
#ifdef DEBUG
        //        Serial.printf("%s in runit count is %ld\n",getName(),mIterationCount);
#endif
    }

}

unsigned long Sched::getSize()
{
    return tTasks.get_size();
}

Sched::Sched() {
}

void Sched::begin() {
    this->mSchedEnabled=true;
}
void Sched::addTask(Task *task)
{
#ifdef DEBUG
    Serial.printf("add called for task, %s %x\n",task->getName().c_str(),task);
#endif
    tTasks.push_back(task);
    return;
};

void Sched::enable() {
    this->mSchedEnabled=true;
}
void Sched::disable() {
    this->mSchedEnabled=false;
}
bool Sched::isEnabled() {
    return this->mSchedEnabled;
}

int ckCnt=0;

void Sched::run()
{
    if(this->mSchedEnabled) {
#ifdef DEBUG
        //                Serial.println("Looking for tasks");
#endif
        for (auto it = tTasks.begin(); it != tTasks.end(); ++it) {
 //       for (it = tTasks.begin(); it != tTasks.end(); ++it){
            Task* currentTask = *it;

#ifdef DEBUGR
            Serial.printf("%s %d task=%x i=%d millis()=%ld Found name=%s, Is enabled? %d\n",__FILE__,__LINE__,currentTask,i++,millis(),currentTask->getName().c_str(),currentTask->isEnabled());
            currentTask->showInit();
#endif
            if(currentTask->isEnabled()) {
                currentTask->runIt(); 
            }
        }
    } else {
#ifdef DEBUG
        Serial.println("Not enabled");
#endif
    }
}

const SimpleList<Task *>& Sched::getTasks() const
{
    return tTasks;
}

