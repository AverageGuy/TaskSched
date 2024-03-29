//#define DEBUG 1
#include "TaskSched.h"
#ifdef DEBUG
#include "esp_debug_helpers.h"
#endif
/*
Task::Task(const voidFuncType & func,   
        int interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
{
    mWith(false);
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;

#ifdef DEBUG
        Serial.printf("long constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };
Task::Task(const voidFuncTypeWith & func,   int interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
{
    mWith(true);
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;

#ifdef DEBUG
        Serial.printf("long constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };
Task::Task(const voidFuncType & func, 
        int interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
    : mProc(func),

        mIterationCount(0),
        mLastStartTime(millis()),
        mRunImmediately(runImmediately),
        mInterval(interval),
        mEnabled(enabled),
        mIterations(iterations),
        mName(name),
        mWith(false)
{
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;
        mLastStartTime=millis();
#ifdef DEBUG
        Serial.printf("int constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };
Task::Task(const voidFuncTypeWith & func,  int interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
    : mProcW(func),
        mIterationCount(0),
        mLastStartTime(millis()),
        mRunImmediately(runImmediately),
        mInterval(interval),
        mEnabled(enabled),
        mIterations(iterations),
        mName(name),
        mWith(true)
{
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;
        mLastStartTime=millis();
#ifdef DEBUG
        Serial.printf("int constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };
Task::Task(const voidFuncType & func,  unsigned long interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
    : mProc(func),
        mIterationCount(0),
        mLastStartTime(millis()),
        mRunImmediately(runImmediately),
        mInterval(interval),
        mEnabled(enabled),
        mIterations(iterations),
        mName(name),
        mWith(false)
{
#ifdef DEBUG
        Serial.printf("For long interval passed in %ld, mInterval became %ld\n",interval,mInterval);
#endif
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;
        mLastStartTime=millis();
#ifdef DEBUG
        Serial.printf("%s instantiated\n",name.c_str());
        Serial.printf("ulong constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };

Task::Task(const voidFuncTypeWith & func,  unsigned long interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
    : mProcW(func),
        mIterationCount(0),
        mLastStartTime(millis()),
        mRunImmediately(runImmediately),
        mInterval(interval),
        mEnabled(enabled),
        mIterations(iterations),
        mName(name),
        mWith(true)
{
#ifdef DEBUG
        Serial.printf("For long interval passed in %ld, mInterval became %ld\n",interval,mInterval);
#endif
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;
        mLastStartTime=millis();
#ifdef DEBUG
        Serial.printf("%s instantiated\n",name.c_str());
        Serial.printf("ulong constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };

Task::Task(const voidFuncType & func,  double interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
    : mProc(func),
        mIterationCount(0),
        mLastStartTime(millis()),
        mRunImmediately(runImmediately),
        mInterval((long)(interval*TASK_SECOND)),
        mEnabled(enabled),
        mIterations(iterations),
        mName(name),
        mWith(false)
{
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;
        mLastStartTime=millis();
#ifdef DEBUG
        Serial.printf("%s instantiated\n",name.c_str());
        Serial.printf("double constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
    };

Task::Task(const voidFuncTypeWith & func,  double interval,
        bool enabled,
        unsigned long iterations,
        String name,
        bool runImmediately)
    : mProcW(func),
        mIterationCount(0),
        mLastStartTime(millis()),
        mRunImmediately(runImmediately),
        mInterval((long)(interval*TASK_SECOND)),
        mEnabled(enabled),
        mIterations(iterations),
        mName(name),
        mWith(true)
{
        mOrig.mEnabled=enabled;
        mOrig.mInterval=interval;
        mOrig.mIterations=iterations;
        mOrig.mRunImmediately=runImmediately;
        mLastStartTime=millis();
        //Serial.println("Befoe call");
        //mProc();
        //Serial.println("After call");
#ifdef DEBUG
        Serial.printf("%s instantiated\n",name.c_str());
        Serial.printf("double constructor for %s called\n",name);
        Serial.printf("%d: Stats for %s, interval = %ld, enabled = %d, iterations = %ld\n",__LINE__,name,interval,enabled,iterations);
#endif
        //mProc=func;
    };
*/
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
    esp_backtrace_print(2);
    Serial.printf("%s %d %s\n",__FILE__,__LINE__,res);
#endif

    return String(res); 
}

String Task::showTaskInfo() {

    char buf[200];
    unsigned long diff = millis() - getLastStartTime();
    String sDiff= formatMS(diff);
    String sInt= formatMS(getInterval());
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
    mRunImmediately=mOrig.mRunImmediately;
#ifdef DEBUG
    Serial.printf("Restarting %s\n",mName.c_str());
#endif
    return;
}

void Task::setCallback(const voidFuncTypeWith & func) {
    mProc=func;
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

bool Task::getRunImmediately(void)
{
    return mRunImmediately;
}

unsigned long Task::getInterval(void)
{
    return mInterval;
}

unsigned long Task::getLastStartTime(void)
{
    return mLastStartTime;
}

unsigned long Task::getIterationCount() 
{
    return mIterationCount;
}

void  Task::runIt() {
#ifdef DEBUG
    Serial.printf("%s %d %s called for runIt\n",__FILE__,__LINE__,mName.c_str());
#endif
    if(!isEnabled()) {
        return;
    }

    if((mRunImmediately && mIterationCount == 0) || (millis() - mLastStartTime) > mInterval){
        mRunImmediately=false; // don't do it again
        unsigned long diff = millis() - getLastStartTime();
        String sDiff= formatMS(diff);
        String sInt= formatMS(getInterval());
#ifdef DEBUG
        Serial.printf("%s %d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__FILE__,__LINE__,getName().c_str().c_str(),isEnabled(),sDiff.c_str(),sInt.c_str(),getRunImmediately());
#endif
        mProc(this);
        ++(mIterationCount);
#ifdef DEBUG
        Serial.printf("%s %d %s in runit iteration count is %ld, max is %ld\n",__FILE__,__LINE__,getName().c_str(),mIterationCount,mIterations);
#endif
        if(mIterationCount>mIterations && mIterations != 0) {
#ifdef DEBUG
            Serial.printf("Iterations exhaused for %s, diabling\n",getName()).c_str();
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

unsigned long Sched::getSize()
{
    return tTasks.size();
}
    /** num = true show status of num tasks, 
     *
    * num = 0 show status of enabled tasks only
    *
    * if taskName != "" ignore num setting
    */
String Sched::displayStatus(int num,String taskName,bool raw) {
    static char printBuffer[1000]; // used to display status 

    char temp[1000];
    strcpy(temp,"");
    std::list<Task*>::iterator it;
    sprintf(printBuffer,"%s","");
    int cnt=0;
    for (it = tTasks.begin(); it != tTasks.end(); ++it){
        Task *currentTask = *it;
        unsigned long diff = millis() - currentTask->getLastStartTime();
        String sDiff= currentTask->formatMS(diff);
        char sint[32];
        if(raw) {
            sprintf(sint,"%d",  currentTask->getInterval());
        } else {
            String sIntx= currentTask->formatMS(currentTask->getInterval());
            strcpy(sint,sIntx.c_str());
        }
        String sInt(sint);
        if(taskName!="") {
            String name = currentTask->getName();
            if(taskName == name) {
                sprintf(temp,"%d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",cnt,currentTask->getName().c_str(),currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediately());
                strcat(printBuffer,temp);
            }
        } else if(!num && currentTask->isEnabled()) {
            if(!currentTask->isEnabled()) {
                //         Serial.printf("Skip %x\n",currentTask);
                continue;
            }
            //                    sprintf(temp,"Task %s, Enabled? %d Diff %s, Interval %s, RI %d\n",currentTask->getName(),currentTask->isEnabled(),sDiff,sInt,currentTask->getRunImmediately());
            cnt++;
            sprintf(temp,"%d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",cnt,currentTask->getName().c_str(),currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediately());
            //    Serial.printf("  Temp %s\n",temp);
            //   Serial.printf("%ld dif %s\n",__LINE__, sDiff.c_str());
            strcat(printBuffer,temp);
        } else if(num) {
            if(cnt>num) {
                break;
            }
            sprintf(temp,"Task %9s,  Enabled? %d, Diff %s, Interval %s, RI %d\n", currentTask->getName().c_str(), currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediately());
            //sprintf(temp,"Task %s, Diff %lu, Interval %lu, enabled %d, RI %d\n",currentTask->getName(),diff,currentTask->getInterval(),currentTask->isEnabled(),currentTask->getRunImmediately());
            //                    Serial.printf("%ld dif %s\n",__LINE__, sDiff.c_str());
            if(strlen(temp)+strlen(printBuffer)<999) {
                strcat(printBuffer,temp);
            }
        }
    }
    String retStr(printBuffer);
    return retStr;
}

Sched::Sched() {
}

void Sched::begin() {
    this->mSchedEnabled=1;
}
void Sched::addTask(Task *task)
{
#ifdef DEBUG
    Serial.printf("add called for task, %s %x\n",task->getName().c_str(),task);
#endif
    tTasks.push_front(task);
    return;
};

void Sched::enable() {
    this->mSchedEnabled=1;
}
void Sched::disable() {
    this->mSchedEnabled=0;
}
int Sched::isEnabled() {
    return this->mSchedEnabled;
}

int ckCnt=0;

void Sched::run()
{
    if(this->mSchedEnabled) {
#ifdef DEBUG
        //                Serial.println("Looking for tasks");
#endif
        std::list<Task*>::iterator it;
        for (it = tTasks.begin(); it != tTasks.end(); ++it){
            Task *currentTask = *it;
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

const std::list<Task *>& Sched::getTasks() const
{
    return tTasks;
}

