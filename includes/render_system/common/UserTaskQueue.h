//
// Created by Walter Rawdanik on 10/16/15.
//

#ifndef MINIENGINE_USERTASKQUEUE_H
#define MINIENGINE_USERTASKQUEUE_H

#include "UserTask.h"
#include "intrusive_list.h"

namespace MiniEngine
{
/*
 *  RenderResourceIdBufferLock describines resource with service side buffer that can have a list of UserTasks assigned to it
 *  It contains RenderResourceIdBuffer and  UserTaskQueueEntryList
 *
 *
 */


    class UserTaskQueueEntry : public rde::intrusive_list_node
    {
    public:

        UserTaskQueueEntry()
        {

        }


        UserTaskQueueEntry(const UserTask &t):task(t)
        {

        }

        void reset()
        {
            task=UserTask();
            next= nullptr;
            prev= nullptr;
        }

        UserTask task;
        uint64_t finishByFrame;
     };




    class RenderResourceIdBufferLock
    {
    public:

        using UserTaskQueueEntryList=rde::intrusive_list<UserTaskQueueEntry>;

        RenderResourceIdBufferLock()
        {



        }

        RenderResourceIdBufferLock(const RenderResourceIdBuffer &buff):buffer(buff)
        {

        }

        RenderResourceIdBuffer buffer;
        UserTaskQueueEntryList list;

    };


    class UserTaskQueue
    {
    public:

        UserTaskQueue(size_t poolSize):mUserTasksPool(poolSize)
        {

        }

        // Returns client side buffer definition

        inline RenderResourceIdBuffer* buffer(RenderResourceId id)
        {
            auto found=mMap.find(id.uid());
            if(found!=mMap.end())
                return &found->second.buffer;

            return nullptr;
        }

        // When user submites a new taks, it is inserted using this method
        // tasks are batched in list of tasks and at the end of the frame synch, are all submited as a continous array of tasks for that buffer
        // the job id is added to the list of jobs id that must finish before X frame

        inline bool insert(const UserTask &task)
        {
            auto found=mMap.find(task.renderResourceId().uid());
            if(found!=mMap.end())
            {
                UserTaskQueueEntry *uTask=mUserTasksPool.create();
                if(uTask!= nullptr)
                {
                    uTask->task=task;
                    found->second.list.push_back(uTask);
                    return true;
                }
            }
            return false;
        }

        // When new buffer is defined, it is inserted using this method

        inline void insert(const RenderResourceIdBuffer &buffer)
        {
            mMap.insert(rde::make_pair(buffer.id.uid(),RenderResourceIdBufferLock(buffer)));
        }



    private:
        using RenderResourceIdBufferMap=rde::hash_map<size_t, RenderResourceIdBufferLock>;

        RenderResourceIdBufferMap mMap;
        ObjectPool<UserTaskQueueEntry> mUserTasksPool;


    };


}

#endif //MINIENGINE_USERTASKQUEUE_H