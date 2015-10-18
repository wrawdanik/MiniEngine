#include "Task.h"
#include "TaskQueue.h"

using namespace MiniEngine;





TaskResult::Result Task::execute(ExecutorId executorId,InterruptPoint interruptPoint)
{
    TaskResult lastResult=mResult;
    lastResult.result=TaskResult::Skipped;

    if(mId.isValid())
    {
        mResult=mHandler(*this,executorId,interruptPoint);
        lastResult=mResult;
    }

    if(mResultQueue!= nullptr)
    {
        mResultQueue->post(Task(mId,mHandler.clientHandler, this->data(),nullptr,lastResult));
    }
    return lastResult.result;
}
