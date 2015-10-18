#ifndef JOB_EXECUTOR_H
#define JOB_EXECUTOR_H

#include "Log.h"
#include <thread>
#include "ExecutorResources.h"
#include "StopTimer.h"

#include "ExecutorResources.h"
#include "blockingconcurrentqueue.h"
#include "TaskQueue.h"

#include <atomic>


using namespace moodycamel;

namespace MiniEngine
{
    class Task;
    class TaskQueue;




    class Executor
    {
    public:


        struct Stats
        {
            Stats()
            {
                reset();
            }
            void reset()
            {
                jobsExecuted=0;
                totalMilliseconds=0;
                busyMilliseconds=0;
                jobsSucesfullyExecuted=0;
                jobsSkipped=0;

            }
            size_t jobsExecuted;
            size_t jobsSucesfullyExecuted;
            size_t jobsSkipped;
            uint64_t totalMilliseconds;
            uint64_t busyMilliseconds;
        };

        using StatsArray=rde::vector<Executor::Stats>;

        Executor(ExecutorId  id, TaskQueue *queue);
        virtual ~Executor();

        inline void setQueue(TaskQueue *queue)
        {
            this->mQueue=queue;
        }
        inline TaskQueue * queue() { return mQueue;}
        inline ExecutorId id() const { return mId;}

        static void executor_func(void * arg);
        static size_t hardwareConcurency();

        Stats join();
        void start();
        void notify();



    private:

        std::atomic_flag mIsRunningInternal;
        std::atomic_flag mIsRunningExternal;
        Log  mLog;

        TaskQueue *mQueue;
        std::thread     *mThread;
        ExecutorId      mId;

        StopTimer mTotalTimer;
        StopTimer mJobTimer;

        mutable Stats mStats;



    };





    
    
}



#endif