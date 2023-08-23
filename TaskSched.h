#ifndef TASKSCHED_H
#define TASKSCHED_H

#include "Arduino.h"
#include <list>
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
        Task(const voidFuncType & func,  unsigned long interval=5000,
                //Task(<void(*)()> func,unsigned long interval=5000,
                bool enabled=false,
                unsigned long iterations=0,
                String name="Unk",
                bool runImmediate=false)
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
                //mProc=func;
                };
                //    Task(*functionPtr)(),mInterval=5000,mEnabled=false,mIterations=0);

    public:
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
        void setIterations(unsigned long newIterations)
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

class Sked
{
    public:
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

        Sked() {
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
