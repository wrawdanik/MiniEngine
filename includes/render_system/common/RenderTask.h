#include <Platform.h>
#include "Status.h"
#include "RenderResourceId.h"
#include "Task.h"
#include "DataTypes.h"
#include "UserTask.h"
#include "RenderDefaults.h"

#ifndef MINIENGINE_RENDERTASK_H_H
#define MINIENGINE_RENDERTASK_H_H

namespace MiniEngine
{
    enum class RenderTaskType : uint32_t
    {
        Nop,
        ResourceRelease,
        ResourceAllocate,
        RenderTarget
    };




    class RenderBackendManagerMemoryTest;

    class TaskResult;
    class RenderManager;





    class RenderTask
    {
    public:
        RenderTask() : type(RenderTaskType::Nop)
        {

        }

        inline static RenderTask* createRenderTask(RenderTask* task,RenderTaskType type, RenderBackendManagerMemoryTest *backendManager,void *param)
        {
            if(task!= nullptr)
            {
                task->backendManager = backendManager;
                task->type = type;
                task->parameter = param;
            }
            return task;
        }

        static TaskResult::Result taskExec(const Task &task,InterruptPoint interruptPoint);

        inline void reset()
        {
            parameter=0;
            type=RenderTaskType::Nop;
            backendManager= nullptr;
        }


        RenderTaskType type;
        RenderBackendManagerMemoryTest *backendManager;
        void *parameter;
    };


    class RenderTargetRequest
    {
    public:
        friend class RenderManager;
        friend class RenderBackendManagerMemoryTest;



    };




    class ResourceRequest
    {
    public:

        friend class RenderManager;
        friend class RenderBackendManagerMemoryTest;

        inline ResourceRequest()
        {
            mData.value=0;
        }

        inline static ResourceRequest * createAllocateRequest(ResourceRequest *request,RenderResourceId resourceId,ResourceType  type,RenderBufferMode  mode,size_t sizeInBytes, size_t instanceCount=RenderDefaults::backendBufferInstances())
        {
            if(request!= nullptr)
            {
                request->mResourceId=resourceId;
                request->setStatus(Status::Code::InProgress);
                request->mData.type = static_cast<size_t>(type);
                request->mData.size = sizeInBytes;
                request->mData.instances = instanceCount;
                request->mData.mode=static_cast<size_t>(mode);
                request->mData.type = static_cast<size_t>(RenderTaskType::ResourceAllocate);
            }
            return request;
        }

        inline static ResourceRequest* createReleaseRequest(ResourceRequest *request,RenderResourceId resourceId)
        {
            if(request!= nullptr)
            {
                request->mResourceId = resourceId;
                request->mData.type = static_cast<size_t>(RenderTaskType::ResourceRelease);
            }
            return request;
        }

        inline void reset()
        {
            mData.value=0;
            mResourceId=RenderResourceId();
            mBuffer= nullptr;
        }

        inline Status::Code status() const { return static_cast<Status::Code>(mData.status); }
        void*   buffer() const { return mBuffer;}

        inline void setStatus(Status::Code status) { mData.status=static_cast<size_t>(status);}

    private:
        union RequestData
        {
            struct
            {
                size_t type:        6;
                size_t status:      4;
                size_t instances:   6;
                size_t mode:        1;
                size_t size:        32;
            };
            uint64_t value;
        };
        RenderResourceId mResourceId;
        RequestData mData;
        unsigned char    *mBuffer;
    };

}
#endif //MINIENGINE_RENDERTASK_H_H
