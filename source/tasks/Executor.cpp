#include "Executor.h"
#include "TaskQueue.h"
#include <future>
#include <thread>

//




using namespace MiniEngine;


Executor::Executor(ExecutorId  id, TaskQueue *queue):mIsRunningInternal(ATOMIC_FLAG_INIT),mIsRunningExternal(ATOMIC_FLAG_INIT)
{
    mId=id;
    mLog.setLocked(false);
    mThread= nullptr;
    mQueue=queue;

}
Executor::~Executor()
{
    if(mThread && mThread->joinable())
        join();

}

size_t Executor::hardwareConcurency()
{
    return (size_t)std::thread::hardware_concurrency();
}

Executor::Stats Executor::join()
{
    if(mThread)
    {
            mIsRunningInternal.clear();
            mIsRunningExternal.clear();

        if (mThread != nullptr && mThread->joinable())
        {
            //_Log("joining thread %d\n",this->mId.value);
            mThread->join();
            //_Log("done joining thread %d\n",this->mId.value);
            delete mThread;
            mThread=nullptr;
        }
        mStats.totalMilliseconds=mTotalTimer.elapsedTimeMilliseconds();

    }
    return mStats;
}
void Executor::start()
{
    if(mThread==nullptr)
    {
        mTotalTimer.restart();

        mIsRunningInternal.test_and_set();
        mIsRunningExternal.test_and_set();

        mThread = new std::thread(
                [this]
                {
                    Executor::executor_func(this);
                });
    }
}


void Executor::notify()
{
    mIsRunningInternal.clear();
    mIsRunningExternal.clear();
}
void Executor::executor_func(void * arg)
{


    Executor *executor=(Executor*)arg;
    executor->mLog.msg("Thread executor id %llu is now alive and waiting  ...\n",executor->id().value);

    TaskArray jobArray;

    while(executor->mIsRunningInternal.test_and_set())
    {
        jobArray.clear();
        executor->queue()->get(jobArray,1,true);

        if (!jobArray.empty())
        {
            executor->mJobTimer.restart();
            auto current = jobArray.begin();
            while (current != jobArray.end())
            {
                executor->mStats.jobsExecuted++;
                TaskResult::Result result=current->execute(executor->id(),InterruptPoint(&executor->mIsRunningExternal));
                if(result==TaskResult::Skipped)
                    executor->mStats.jobsSkipped++;
                else if (result==TaskResult::Success)
                    executor->mStats.jobsSucesfullyExecuted++;
                ++current;
            }
            executor->mStats.busyMilliseconds+=executor->mJobTimer.elapsedTimeMilliseconds();
        }
    }
    executor->mLog.msg("Thread executor %llu is terminating ...\n",executor->id().value);
}



