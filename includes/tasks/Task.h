#ifndef TASK_H
#define TASK_H

#include "ExecutorResources.h"
#include "rdestl.h"
#include <functional>


namespace MiniEngine
{
    class TaskQueue;
    class JobHandler;
    class Task;

    class TaskResult
    {
    public:
        enum Result
        {
            Success,
            Failure,
            Skipped,
            Unknown,
            Scheduled
        };




        TaskResult()
        {
            result=Unknown;
        }

        TaskResult(ExecutorId id, Result result)
        {
            executorId=id;
            this->result=result;
        }

        ExecutorId executorId;
        Result result;
    };

    class Task
    {


    public:

        using TaskHandler=std::function<TaskResult::Result(const Task &,InterruptPoint interruptPoint)> ;



        Task()= default;

        Task(TaskId taskId, TaskHandler handler,void *data, TaskQueue *resultQueue=nullptr,TaskResult result=TaskResult()):mResult(result),mHandler(handler)
        {
            mId=taskId;
            mHandler=handler;
            mResultQueue=resultQueue;
            mData=data;
        }

        inline TaskQueue* resultQueue() const
        {
            return mResultQueue;
        }

        inline TaskHandler handler() const
        {
            return mHandler.clientHandler;
        }

        inline TaskResult result() const
        {
            return mResult;
        }

        void* data() const
        {
            return mData;
        }


        TaskResult::Result execute(ExecutorId executorId,InterruptPoint  interruptPoint);

        inline TaskId id() const
        {
            return mId;
        }
    private:


        struct InternalTaskHandler
        {
            InternalTaskHandler()= default;

            InternalTaskHandler(TaskHandler handler):clientHandler(handler)
            {

            }

            TaskResult operator()(const Task &task,ExecutorId id,InterruptPoint interruptPoint) const
            {
                return TaskResult(id,clientHandler(task,interruptPoint));
            }

            TaskHandler clientHandler;

        };

        void *mData;
        TaskId mId;
        TaskQueue *mResultQueue;
        TaskResult mResult;
        InternalTaskHandler mHandler;
    };

    using TaskArray=rde::vector<Task> ;

}



#endif