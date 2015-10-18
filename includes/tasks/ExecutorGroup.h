//
// Created by Walter Rawdanik on 10/3/15.
//

#ifndef MINIENGINE_EXECUTORGROUP_H
#define MINIENGINE_EXECUTORGROUP_H

#include "Executor.h"

namespace MiniEngine
{

    class ExecutorGroup
    {
    public:

        ExecutorGroup(ExecutorGroupId id, TaskQueue *queue);

        ExecutorGroup(ExecutorGroupId id,size_t threadCount, TaskQueue *queue);
        virtual ~ExecutorGroup();

        Executor::StatsArray stop();
        void start();


        inline ExecutorGroupId id() const
        {
            return mId;
        }

        inline size_t size() const
        {
            return mExecutors.size();
        }

    private:


        void create(size_t count);

        using ExecutorArray=rde::vector<Executor*> ;


        size_t  mCount;
        TaskQueue *mQueue;
        ExecutorArray mExecutors;
        ExecutorGroupId mId;
    };







}


#endif //MINIENGINE_EXECUTORGROUP_H
