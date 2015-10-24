#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "RenderResourceId.h"
#include "Status.h"
#include "ObjectPool.h"
#include "RenderTask.h"
#include "UserTask.h"
#include "UserTaskQueue.h"
#include "RenderTarget.h"
#include "RenderDefaults.h"

namespace MiniEngine
{
    struct RenderManagerDescriptor
    {
        size_t InitialCommunicationObjectsPoolSize = 512;
        size_t CommunicationObjectsPoolGrowth = 256;
        size_t InitialCommunicationQueueSize = 256;

    };

    class TaskQueue;
    class Executor;





    class RenderManager : protected IntrusiveTracked::Creator
    {

        friend class RenderManagerProxy;

    public:

        RenderManager(const RenderManagerDescriptor &descriptor,RenderTargetPtr defaultTarget);

        virtual ~RenderManager();

        RenderResourceId allocateResource
                (ResourceType type, RenderBufferMode mode, size_t sizeInBytes, RenderBufferSource source = RenderBufferSource::Server, size_t instanceCount = RenderDefaults::backendBufferInstances());

        RenderResourceIdBuffer memoryBuffer(RenderResourceId id);


        UserTaskFuture submitUserTask(const UserTask &task);


        inline RenderTargetPtr defaultTarget()
        {
            return mDefaultTarget;
        }

        RenderManagerDescriptor descriptor() const
        { return mDescriptor; }


        virtual void release(IntrusiveTracked *object);

    protected:



        RenderManager()
        {

        }

        void releaseResource(RenderResourceId id);


    private:

        inline uint32_t nextResourceId()
        {
            return ++mLastResourceId;
        }

        inline uint32_t nextTaskId()
        {
            return ++mLastTaskId;
        }

        Status::Code userTaskCode(TaskId task, bool discard=false);

        void processBackendUpdates(bool blocking = false);

        /*
         Create an intrusive list associated with RenderResourceIdQueueEntry
         Create a pool of RenderResourceIdQueueEntry
         Create master list of RenderResourceIdQueueEntry.

         RenderResourceIdData will be kept in RenderResourceIdBufferMap as the value.
         It will keep RenderResourceIdBuffer.
         RenderResourceIdQueueEntry - first entry to be executed

         for a new task, append entry to the list of RenderResourceIdQueueEntry and if RenderResourceIdData.RenderResourceIdQueueEntry
         is empty, assign that entry there as well.

         When processing new tasks executions, walk the list and find the corresponding RenderResourceIdQueueEntry base on task id.
         Remove it from the entry set and then submit another task on that entry list.

        */



        using RenderResourceIdBufferMap=rde::hash_map<size_t, RenderResourceIdBuffer>;
        using RenderTaskTrackMap=rde::hash_map<TaskId,Status::Code>;





        RenderResourceIdBufferMap mResourceMap;


        RenderManagerDescriptor mDescriptor;

// communication pools

        ObjectPool<RenderTask> mBackendTaskPool;
        ObjectPool<ResourceRequest> mResourceRequestPool;



        Executor *mRenderer;
        TaskQueue *mRequestQueue;
        TaskQueue *mResponseQueue;

        TaskArray mTaskBuffer;

        RenderBackendManagerMemoryTest *mBackendManager;

        uint32_t mLastResourceId;

        TaskId mLastProcessedTaskId;
        uint32_t mLastTaskId;

        RenderTaskTrackMap mRenderTaskTrackMap;

        RenderTargetPtr mDefaultTarget;
    };


}


#endif