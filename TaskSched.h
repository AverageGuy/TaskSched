#ifndef TASKSCHED_H
#define TASKSCHED_H

#include "Arduino.h"
#include <list>
#define TASK_SECOND 1000
#define TASK_MINUTE 60*TASK_SECOND
using namespace std;


typedef std::function<void(void)> voidFuncType;

struct savedInitial {
    voidFuncType mProc;
    unsigned long mInterval;
    bool mEnabled;
    bool mRunImmediate;
    unsigned long mIterations;
};
typedef savedInitial initialState;



class Task
{
    public:
        Task(const voidFuncType & func,  int interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false);
        Task(const voidFuncType & func,  long interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false);
        Task(const voidFuncType & func,  unsigned long interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false);
        Task(const voidFuncType & func,  double interval=5.0, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false);
        // interval is in seconds for float and ms for long
        bool isFirstIteration();
        bool isLastIteration();
        bool fRunImmediate();
        void restart();
        void enable();
        void disable();
        bool isEnabled();
        void setCallback(const voidFuncType &);
        void  runIt();
        void showInit();
        String getName();
        unsigned long mIterationCount;
        void setInterval(unsigned long newInterval);
        void setIterations(unsigned long newIterations);
        void setImmediate(bool);
        void showTaskInfo();
        unsigned long getInterval(void);
        unsigned long getRunImmediate(void);
        unsigned long getLastStartTime(void);
        String formatMS(unsigned long milliseconds);
    private:
        bool mRunImmediate;
        bool doShow=true;
        unsigned long mLastStartTime;
        initialState mOrig;
        String mName;
        voidFuncType mProc;
        unsigned long mInterval;
        bool mEnabled;
        unsigned long mIterations;
        unsigned long mRunItCalled=0;
        double passedInterval=-1.0;
};

class Sched
{
    public:
        // sample setter and getter
        void set(unsigned long now)
        {
            test = now;
        }

        unsigned long get()
        {
            return test;
        }
        unsigned long getSize()
        {
            return tTasks.size();
        }
        String displayStatus(bool all=0,String taskName="") {
            // all = true show status of all tasks, 
            // all = false show status of enabled tasks only
            // if taskNmae != "" ignore all setting
            static char printBuffer[1000]; // used to display status 

            char temp[1000];
            strcpy(temp,"");
            list<Task*>::iterator it;
            sprintf(printBuffer,"");
            int i=0;
            //Serial.printf("=== %ld Size task list %d\n",__LINE__,getSize());
            for (it = tTasks.begin(); it != tTasks.end(); ++it){
                Task *currentTask = *it;
                unsigned long diff = millis() - currentTask->getLastStartTime();
                String sDiff= currentTask->formatMS(diff);
                String sInt= currentTask->formatMS(currentTask->getInterval());
             //   Serial.printf("-- interval %ld %s %s\n",currentTask->getInterval(),sInt,sInt.c_str());
                if(taskName!="") {
                    String name = currentTask->getName();
                    if(taskName == name) {
                        sprintf(temp,"%ld %d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__LINE__,i,currentTask->getName(),currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediate());
                        strcat(printBuffer,temp);
                    }
                } else if(!all && currentTask->isEnabled()) {
              //      Serial.printf("Looking at %x\n",currentTask);
                    if(!currentTask->isEnabled()) {
               //         Serial.printf("Skip %x\n",currentTask);
                        continue;
                    }
//                    sprintf(temp,"Task %s, Enabled? %d Diff %s, Interval %s, RI %d\n",currentTask->getName(),currentTask->isEnabled(),sDiff,sInt,currentTask->getRunImmediate());
                    i++;
                    sprintf(temp,"%ld %d Task %s, Enabled? %d, Diff %s, Interval %s, RI %d\n",__LINE__,i,currentTask->getName(),currentTask->isEnabled(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediate());
                //    Serial.printf("  Temp %s\n",temp);
                 //   Serial.printf("%ld dif %s\n",__LINE__, sDiff.c_str());
                    strcat(printBuffer,temp);
                } else if(all) {
                    sprintf(temp,"%ld Task %s, Diff %s, Interval %s, RI %d\n",__LINE__,currentTask->getName(),sDiff.c_str(),sInt.c_str(),currentTask->getRunImmediate());
                    //sprintf(temp,"Task %s, Diff %lu, Interval %lu, enabled %d, RI %d\n",currentTask->getName(),diff,currentTask->getInterval(),currentTask->isEnabled(),currentTask->getRunImmediate());
//                    Serial.printf("%ld dif %s\n",__LINE__, sDiff.c_str());
                    strcat(printBuffer,temp);
                }
                //currentTask->showInit();
            }
            String retStr(printBuffer);
            return retStr;
        }

        Sched() {
        }
        void begin() {
            this->mSchedEnabled=1;
        }
        void addTask(Task *task)
        {
#ifdef DEBUG
            Serial.printf("add called for task, %s\n",task->getName());
#endif
            tTasks.push_front(task);
            return;
        };

        void enable() {
            this->mSchedEnabled=1;
        }
        void disable() {
            this->mSchedEnabled=0;
        }
        int isEnabled() {
            return this->mSchedEnabled;
        }

        int ckCnt=0;
        void run()
        {
            if(this->mSchedEnabled) {
#ifdef DEBUG
                //                Serial.println("Looking for tasks");
#endif
                list<Task*>::iterator it;
                int i=0;
                for (it = tTasks.begin(); it != tTasks.end(); ++it){
                    Task *currentTask = *it;
#ifdef DEBUG
                    //     Serial.printf("%d %ld Found %s, Is? %d\n",i++,millis(),currentTask->getName(),currentTask->isEnabled());
                    currentTask->showInit();
#endif
                    /*
                       if(ckCnt++<20) {
                       String en = currentTask->isEnabled()?"true":"false";
                       Serial.printf("%s is enabled?  %s\n",currentTask->getName(),en);
                       }
                       */
                    if(currentTask->isEnabled()) {
                        currentTask->runIt(); 
                    }
                }
            } else {
#ifdef DEBUG
                //                Serial.println("Not enabled");
#endif
            }
        }

    private:
        unsigned long test;
        list<Task *> tTasks;
        int mSchedEnabled;

};

#endif
