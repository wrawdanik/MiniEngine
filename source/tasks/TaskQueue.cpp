#include "TaskQueue.h"
#include "fixed_vector.h"
#include "Executor.h"

using namespace MiniEngine;

TaskQueue::TaskQueue(size_t capacity):mCommonBuffer(capacity)
{
    mLog.setLocked(false);
    mCapacity=capacity;
}
TaskQueue::~TaskQueue()
{
    
}
size_t TaskQueue::capacity() const
{
    return mCapacity;
}

size_t TaskQueue::size() const
{
    return (size_t)mCommonBuffer.size_approx();

}

bool TaskQueue::post(const Task &task)
{
    if(task.result().result==TaskResult::Unknown)
        return mCommonBuffer.enqueue(Task(task.id(),task.handler(),task.data(),task.resultQueue(),TaskResult(ExecutorId(),TaskResult::Result::Scheduled)));
    else
        return mCommonBuffer.enqueue(task);
}
bool TaskQueue::post(const TaskArray &tasks)
{
    using FixedTaskArray=rde::fixed_vector<Task,32,true>;
    FixedTaskArray taskArray;

    for(auto task:tasks)
        if(task.result().result==TaskResult::Unknown)
            taskArray.push_back(Task(task.id(),task.handler(),task.data(),task.resultQueue(),TaskResult(ExecutorId(),TaskResult::Result::Scheduled)));

    return mCommonBuffer.enqueue_bulk(taskArray.begin(),taskArray.size());

}
Task TaskQueue::get(bool blocking)
{
    Task task;
    if(blocking)
        mCommonBuffer.wait_dequeue(task);
    else
        mCommonBuffer.try_dequeue(task);
    return task;
}
size_t TaskQueue::get(TaskArray &tasks,size_t maxCount,bool blocking)
{
    tasks.resize(maxCount);

    size_t jobCount=0;

    if(blocking)
        jobCount=mCommonBuffer.wait_dequeue_bulk(tasks.begin(),maxCount);
    else
        jobCount=mCommonBuffer.try_dequeue_bulk(tasks.begin(),maxCount);

    tasks.resize(jobCount);

    return jobCount;
}


