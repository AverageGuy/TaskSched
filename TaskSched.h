#ifndef TASKSCHED_H
#define TASKSCHED_H

#include "Arduino.h"
#include <list>
#define TASK_SECOND 1000
#define TASK_MINUTE 60
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
        Task(const voidFuncType & func,  unsigned long interval=5000, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false);
        Task(const voidFuncType & func,  double interval=5.0, bool enabled=false, unsigned long iterations=0, String name="Unk", bool runImmediate=false);
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
        String showTaskInfo();
        unsigned long getInterval(void);
        unsigned long getRunImmediate(void);
        unsigned long getLastStartTime(void);
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
        String displayStatus(bool all=0) {
            // all = true show status of all tasks, 
            // all = false show status of enabled tasks only
            static char printBuffer[1000]; // used to display status 
                                           
            char temp[100];
            list<Task*>::iterator it;
            sprintf(printBuffer,"");
            int i=0;
            for (it = tTasks.begin(); it != tTasks.end(); ++it){
                Task *currentTask = *it;
                unsigned long diff = millis() - currentTask->getLastStartTime();
                if(!all && currentTask->isEnabled()) {
                    sprintf(temp,"Task %s, Diff %lu, Interval %lu, RI %d\n",currentTask->getName(),diff,currentTask->getInterval(),currentTask->getRunImmediate());
                }
                if(all) {
                    sprintf(temp,"Task %s, Diff %lu, Interval %lu, enabled %d, RI %d\n",currentTask->getName(),diff,currentTask->getInterval(),currentTask->isEnabled(),currentTask->getRunImmediate());
                }
                strcat(printBuffer,temp);
                //currentTask->showInit();
            }
            String retStr(printBuffer);
            return retStr;
        }

        Sched() {
        }
        void begin() {
            this->mEnabled=1;
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
            this->mEnabled=1;
        }
        void disable() {
            this->mEnabled=0;
        }
        int isEnabled() {
            return this->mEnabled;
        }
        
        void run()
        {
            if(this->mEnabled) {
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
        int mEnabled;

};

#endif
