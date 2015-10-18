//
// Created by Walter Rawdanik on 10/14/15.
//


#ifndef MINIENGINE_BUFFERCONTENTTASK_H
#define MINIENGINE_BUFFERCONTENTTASK_H

#include <functional>
#include "Status.h"
#include "RenderResource.h"
#include "Task.h"

namespace MiniEngine
{
    class RenderManager;
    class RenderResourceIdBuffer;



    class UserTask
    {
    public:

        enum class Type
        {
            Asynchronous,
            WaitForRenderTask
        };

        UserTask()=default;

        UserTask(Task::TaskHandler handler,void *data= nullptr,RenderResourceId resourceId=RenderResourceId(),
                 Type type=Type::Asynchronous):mHandler(handler),mData(data),mType(type),mResourceId(resourceId)
        {

        }

        inline Task::TaskHandler handler() const
        {
            return mHandler;
        }
        inline void* data() const
        {
            return mData;
        }
        inline Type type() const
        {
            return mType;
        }
        inline RenderResourceId renderResourceId() const
        {
            return mResourceId;
        }

    private:

        Task::TaskHandler mHandler;
        UserTask::Type mType;
        void* mData;
        RenderResourceId mResourceId;
    };

    class UserTaskFutureImpl: public IntrusiveTracked
    {
    public:
        friend class RenderManager;

        Status::Code code()
        {
            if(mCode==Status::Code::InProgress)
            {
                return mManagerProxy.renderTaskStatusCode(mTaskId);
            }
            return Status::Code::Success;
        }

        void* data() const
        {
            return mData;
        }

        RenderResourceId id() const
        {
            return mId;
        }

    private:
        UserTaskFutureImpl(RenderManager *manager,TaskId taskId,void *data,RenderResourceId id,Status::Code code):mManagerProxy(manager),
                                                                                                                  mTaskId(taskId),mId(id),mCode(code),mData(data)
        {

        }

        ~UserTaskFutureImpl()
        {
            mManagerProxy.renderTaskStatusCode(mTaskId,true);
        }
        RenderManagerProxy mManagerProxy;
        TaskId mTaskId;
        RenderResourceId mId;
        Status::Code mCode;
        void *mData;
    };


    using UserTaskFuture=intrusive_ptr<UserTaskFutureImpl>;


}
#endif //MINIENGINE_BUFFERCONTENTTASK_H
