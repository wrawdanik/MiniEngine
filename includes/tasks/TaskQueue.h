#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "Log.h"
#include "Task.h"
#include "ExecutorResources.h"
#include "blockingconcurrentqueue.h"

using namespace moodycamel;

namespace MiniEngine
{
    class Executor;
    
    class TaskQueue
    {
    public:
        
        TaskQueue(size_t capacity=128);
        ~TaskQueue();
        
        bool post(const Task &task);
        bool post(const TaskArray &tasks);
        Task get(bool blocking=false);
        size_t get(TaskArray &tasks,size_t maxCount,bool blocking=false);
        size_t size() const;
        size_t capacity() const;
    private:

        Log  mLog;
        size_t mCapacity;


        BlockingConcurrentQueue<Task> mCommonBuffer;



        
    };
    

    

    
}



#endif