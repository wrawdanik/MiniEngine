
#include "RenderManager.h"
#include "Log.h"
#include "RenderBackendManagerMemoryTest.h"

using namespace MiniEngine;


RenderManager::RenderManager(const RenderManagerDescriptor &descriptor):mDescriptor(descriptor),
                                                                        mBackendTaskPool(descriptor.InitialCommunicationObjectsPoolSize,descriptor.CommunicationObjectsPoolGrowth),
                                                                        mResourceRequestPool(descriptor.InitialCommunicationObjectsPoolSize,descriptor.CommunicationObjectsPoolGrowth),
                                                                        mRequestQueue(descriptor.InitialCommunicationQueueSize),
                                                                        mResponseQueue(descriptor.InitialCommunicationQueueSize)
{
    mLastResourceId=mLastTaskId=0;
    mBackendManager=new RenderBackendManagerMemoryTest();
    mTaskBuffer.reserve(descriptor.InitialCommunicationQueueSize);
}


void RenderManager::releaseResource(RenderResourceId id)
{
    RenderResourceIdBufferMap::iterator found = mResourceMap.find(id.uid());
    if (found != mResourceMap.end())
    {
        if(found->second.id.source()==RenderBufferSource::Client)
        {
            delete found->second.buffer;
            mResourceMap.erase(found);
        }
        else
        {
            bool succesfull=false;
            ResourceRequest *request=nullptr;
            RenderTask *renderTask=nullptr;

            request = ResourceRequest::createReleaseRequest(mResourceRequestPool.create(), found->second.id);
            if(request!= nullptr)
            {
                renderTask = RenderTask::createRenderTask(mBackendTaskPool.create(), RenderTaskType::ResourceRelease,
                                                          mBackendManager, request);
                if(renderTask!= nullptr)
                {
                    succesfull=mRequestQueue.post(Task(TaskId(nextTaskId()), RenderTask::taskExec, renderTask));
                    if(succesfull)
                        mResourceMap.erase(found);
                }
            }
            if(!succesfull)
            {
                mResourceRequestPool.destroy(request);
                mBackendTaskPool.destroy(renderTask);
            }
        }
    }
}


RenderResourceId RenderManager::allocateResource(ResourceType type,RenderBufferMode mode, size_t sizeInBytes,RenderBufferSource  source, size_t instanceCount)
{
    RenderResourceId id = RenderResourceId(0, nextResourceId(), type,mode,source);

    if(source==RenderBufferSource::Client)
    {
        unsigned char *bufferData=new (std::nothrow) unsigned char[sizeInBytes];

        if(bufferData!= nullptr)
            id.update(true,true);
        mResourceMap.insert(rde::make_pair(id.uid(),RenderResourceIdBuffer(id,bufferData,sizeInBytes)));
    }
    else
    {
        bool succesfull=false;
        ResourceRequest *request=nullptr;
        RenderTask *renderTask=nullptr;

        request = ResourceRequest::createAllocateRequest(mResourceRequestPool.create(),id, type,mode, sizeInBytes, instanceCount);
        if(request!= nullptr)
        {
            renderTask = RenderTask::createRenderTask(mBackendTaskPool.create(),
                                                                  RenderTaskType::ResourceAllocate, mBackendManager,
                                                                  request);
            if(renderTask!= nullptr)
            {
                succesfull=mRequestQueue.post(Task(TaskId(nextTaskId()), RenderTask::taskExec, renderTask));
                if(succesfull)
                    mResourceMap.insert(rde::make_pair(id.uid(), RenderResourceIdBuffer(id)));
            }
        }
        if(!succesfull)
        {
            mResourceRequestPool.destroy(request);
            mBackendTaskPool.destroy(renderTask);
        }
    }

    return id;
}



RenderResourceIdBuffer RenderManager::memoryBuffer(RenderResourceId id)
{
    RenderResourceIdBufferMap::iterator found = mResourceMap.find(id.uid());
    if (found != mResourceMap.end())
    {
        if(found->second.id.mode()==RenderBufferMode::Dynamic)
        {
            if (found->second.id.isMapped() || !found->second.id.isValid() || found->second.id.source()==RenderBufferSource::Client)
                return found->second;
            else
            {
                while (true)
                {
                    found = mResourceMap.find(id.uid());
                    if (found == mResourceMap.end() || found->second.id.isMapped() || !found->second.id.isValid())
                    {
                        break;
                    }
                    processBackendUpdates(true);
                }
                return found->second;
            }
        }
    }
    return RenderResourceIdBuffer();
}



void RenderManager::processBackendUpdates(bool blocking)
{
    size_t count=mResponseQueue.get(mTaskBuffer,32,blocking);
    if(count>0)
    {
        for(auto &task:mTaskBuffer)
        {
            mLastProcessedTaskId=task.id();
            if(task.result().result==TaskResult::Success)
            {
                RenderTask *renderTask=static_cast<RenderTask*>(task.data());
                if(renderTask!= nullptr)
                {
                    if(renderTask->type!=RenderTaskType::RenderTarget)
                    {
                        ResourceRequest *request=static_cast<ResourceRequest*>(renderTask->parameter);
                        if(request!= nullptr)
                        {
                            if(renderTask->type==RenderTaskType::ResourceAllocate)
                            {
                                RenderResourceIdBufferMap::iterator found = mResourceMap.find(request->mResourceId.uid());
                                if (found != mResourceMap.end())
                                {
                                    found->second.buffer = request->mBuffer;
                                    found->second.sizeInBytes=request->mData.size;
                                    found->second.id.update(request->status() == Status::Code::Success,
                                                            found->second.buffer != nullptr);
                                }
                            }
                            else
                            {
                                mResourceMap.erase(request->mResourceId.uid());
                            }
                        }
                        mResourceRequestPool.destroy(request);
                    }
                    else
                    {

                    }
                }
                mBackendTaskPool.destroy(renderTask);
            }
        }
    }
}

UserTaskFuture RenderManager::submitUserTask(const UserTask &task)
{
    UserTaskFuture future=new UserTaskFutureImpl(this,TaskId(),task.data(),task.renderResourceId(),Status::Code::InProgress);

    return future;
}
/*
BufferContentTaskFuture RenderManager::submitBufferContentTask(const BufferContentTask &task, RenderResourceId resourceId)
{
    BufferContentTaskFuture future=new BufferContentTaskFutureImpl(this,TaskId(),resourceId, Status::Code::InProgress);

    if(resourceId.isValid())
    {
        // if this is client resource just handle is synchronously
        if(resourceId.source()==RenderBufferSource::Client)
        {
            //future.mCode=Status::Code::Completed;
        }
        else
        {
            ResourceContentRequest *renderTask=ResourceContentRequest::createContentRequest(mResourceContentRequestPool.create(),
            task,resourceId);

            if(renderTask!= nullptr)
            {
                TaskId taskId(nextTaskId());
                if (mRequestQueue.post(Task(taskId, RenderTask::taskExec, renderTask)))
                {
                    mRenderTaskTrackMap.insert(rde::make_pair(taskId,Status::Code::InProgress));
                }
                else
                {
                    future->mCode = Status::Code::CommunicationError;
                }
            }
            else
            {
                future->mCode=Status::Code::OutOfMemory;
            }
        }
    }
    return future;
}
*/


Status::Code RenderManager::userTaskCode(TaskId taskId,bool discard)
{
    if(taskId.isValid())
    {
        auto found = mRenderTaskTrackMap.find(taskId);
        if(found != mRenderTaskTrackMap.end())
        {
            Status::Code code=found->second;
            if(discard)
            {
                code = Status::Code::Success;
            }
            else
            {
                if (found->second == Status::Code::InProgress)
                {
                    processBackendUpdates(false);
                    found = mRenderTaskTrackMap.find(taskId);
                    code=found->second;
                }
            }
            if(code!=Status::Code::InProgress)
                mRenderTaskTrackMap.erase(found);
            return code;
        }
    }
    return Status::Code::InvalidState;
}