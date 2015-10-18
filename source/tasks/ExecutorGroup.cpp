//
// Created by Walter Rawdanik on 10/3/15.
//

#include "ExecutorGroup.h"

using namespace MiniEngine;


ExecutorGroup::ExecutorGroup(ExecutorGroupId id, TaskQueue *queue):ExecutorGroup(id,Executor::hardwareConcurency(),queue)
{

}


ExecutorGroup::ExecutorGroup(ExecutorGroupId id,size_t threadCount, TaskQueue *queue):mId(id)
{
    mQueue=queue;
    mCount=threadCount;

}


ExecutorGroup::~ExecutorGroup()
{
    stop();
}

Executor::StatsArray ExecutorGroup::stop()
{
    Executor::StatsArray stats;

    if(!mExecutors.empty())
    {

        for (auto executor:mExecutors)
        {
            executor->notify();
        }

        for (size_t i = 0; i < mExecutors.size(); i++)
        {
            mQueue->post(Task());
            mQueue->post(Task());
        }

        for (auto executor:mExecutors)
        {
            stats.push_back(executor->join());
        }

        for (auto executor:mExecutors)
        {
            delete executor;
        }

        mExecutors.clear();
    }

    return stats;
}
void ExecutorGroup::start()
{
    if(!mExecutors.empty())
    {
        stop();
    }
    create(mCount);

    for(auto executor:mExecutors)
    {
        executor->start();
    }
}


void ExecutorGroup::create(size_t count)
{
    uint32_t tId=0;

    for(size_t i=0;i<count;i++)
    {
        mExecutors.push_back(new Executor(ExecutorId(++tId,mId),mQueue));
    }
}