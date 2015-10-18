#define CATCH_CONFIG_MAIN

#include "Executor.h"
#include "Task.h"
#include "ExecutorGroup.h"




#include "catch.hpp"
#include "MathUtils.h"
#include <future>

using namespace MiniEngine;

/*
 *  Create list of X executors ( threads)
 *  Create list of Y float buffers
 *  Execute that algorithm 10 000 times and measure time it took to execute it.
 *  Verify that buffers contain their original values at the end of all transactions.
 *
 *  Execute the same algorirhm  linearly ( using this thread) 10 000 times  and measure how long it took.
 *
 */


using namespace MiniEngine;

using FloatBufferArray=rde::vector<float>;


TaskResult::Result taskExec(const Task &task,InterruptPoint interruptPoint)
{
    size_t loopCount=100;

    FloatBufferArray *array=(FloatBufferArray*)task.data();

    //_Log("Executing job add with executordId %d\n",executorId.value);

    if(array!=0)
    {
        for(size_t i=0;i<loopCount;i++)
        {
            FloatBufferArray::iterator current = array->begin();

            while (current != array->end())
            {

                *current = *current + 5;
                ++current;
            }

        }



        for(size_t i=0;i<loopCount;i++)
        {
            FloatBufferArray::iterator current = array->begin();

            while (current != array->end())
            {
                *current = *current - 1;
                ++current;
            }

        }
    }

    return TaskResult::Success;
}


TEST_CASE("Executor_TestExecution")
{


    for(int z=0;z<1;z++)
    {

        size_t loopCount = 1500;
        constexpr size_t bufferCount = 27;
        size_t bufferSize = 256 * 8;


        TaskQueue *mainQueue = new TaskQueue();
        TaskQueue *responseQueue = new TaskQueue();
        ExecutorGroup *group = new ExecutorGroup(ExecutorGroupId(0), Executor::hardwareConcurency() + 1, mainQueue);

        //ExecutorGroup  *group=new ExecutorGroup(ExecutorGroupId(0),4,mainQueue);


        FloatBufferArray buffersThreaded[bufferCount];
        FloatBufferArray buffersSerial[bufferCount];

        for (int i = 0; i < bufferCount; i++)
        {
            for (size_t sb = 0; sb < bufferSize; sb++)
            {
                buffersThreaded[i].push_back((float) 1);
                buffersSerial[i].push_back((float) 1);
            }
        }

        group->start();


        int jobCounter = 0;
        StopTimer timer;

        for (size_t i = 0; i < loopCount; i++)
        {
            for (size_t j = 0; j < bufferCount; j++)
            {
                FloatBufferArray *buffer = &(buffersThreaded[j]);
                mainQueue->post(Task(TaskId(++jobCounter), taskExec, buffer, responseQueue));
            }

            size_t responseCount = bufferCount;
            while (responseCount)
            {
                Task job = responseQueue->get(true);
                CHECK(job.result().result == TaskResult::Success);
                //
                responseCount--;
            }

        }

        uint64_t duration = timer.elapsedTimeMilliseconds();

        _Log("\n\nThreaded job finished in %d milliseconds\n\n", duration);

        Executor::StatsArray stats = group->stop();


        timer.restart();
        for (size_t i = 0; i < loopCount; i++)
        {
            for (size_t j = 0; j < bufferCount; j++)
            {
                FloatBufferArray *buffer = &(buffersSerial[j]);
                Task job(TaskId(++jobCounter), taskExec, buffer, responseQueue);
                taskExec(job,InterruptPoint());
            }
        }
        duration = timer.elapsedTimeMilliseconds();

        _Log("\n\nSerial job finished in %d milliseconds\n\n", duration);


        int valueCount = 0;

        for (size_t j = 0; j < bufferCount; j++)
        {
            for (size_t w = 0; w < bufferSize; w++)
            {
                CHECK(buffersSerial[j][w] == buffersThreaded[j][w]);
            }
            valueCount++;
        }

        size_t totalJobsExecuted = 0;

        for (auto stat:stats)
        {
            //printf(" Executor jobsExecuted %lu jobsSucesfullyExecuted %lu  jobsSkipped %llu totalMilliseconds %llu busyMilliseconds %lu\n",
            //       stat.jobsExecuted, stat.jobsSucesfullyExecuted, stat.jobsSkipped, stat.totalMilliseconds,
            //       stat.busyMilliseconds);
            totalJobsExecuted += stat.jobsExecuted;
        }


        printf("\ntotal jobs executed %lu", totalJobsExecuted);

        delete group;
        delete mainQueue;
        delete responseQueue;



    }

}


TEST_CASE("Executor_TestExecution_Asynch")
{


    for(int z=0;z<1;z++)
    {

        size_t loopCount = 1500;
        constexpr size_t bufferCount = 27;
        size_t bufferSize = 256 * 8;

        std::vector<std::future<TaskResult::Result>> futures;
        futures.reserve(bufferCount);


        std::vector<Task> tasks;

        FloatBufferArray buffersThreaded[bufferCount];
        FloatBufferArray buffersSerial[bufferCount];

        for (int i = 0; i < bufferCount; i++)
        {
            for (size_t sb = 0; sb < bufferSize; sb++)
            {
                buffersThreaded[i].push_back((float) 1);
                buffersSerial[i].push_back((float) 1);
            }
        }


        int jobCounter = 0;
        StopTimer timer;

        for (size_t i = 0; i < loopCount; i++)
        {
            for (size_t j = 0; j < bufferCount; j++)
            {
                FloatBufferArray *buffer = &(buffersThreaded[j]);

                Task job(TaskId(++jobCounter), taskExec, buffer, nullptr);
                tasks.push_back(job);
                futures.push_back(std::async(launch::async,taskExec,tasks.back(),InterruptPoint()));
            }

            size_t responseCount = bufferCount;
            for(auto &e : futures)
            { e.get();
            }
            futures.clear();

        }

        uint64_t duration = timer.elapsedTimeMilliseconds();

        _Log("\n\nAsynch Threaded job finished in %d milliseconds\n\n", duration);



        timer.restart();
        for (size_t i = 0; i < loopCount; i++)
        {
            for (size_t j = 0; j < bufferCount; j++)
            {
                FloatBufferArray *buffer = &(buffersSerial[j]);
                Task job(TaskId(++jobCounter), taskExec, buffer, nullptr);
                taskExec(job,InterruptPoint());
            }
        }
        duration = timer.elapsedTimeMilliseconds();

        _Log("\n\nSerial job finished in %d milliseconds\n\n", duration);


        int valueCount = 0;

        for (size_t j = 0; j < bufferCount; j++)
        {
            for (size_t w = 0; w < bufferSize; w++)
            {
                CHECK(buffersSerial[j][w] == buffersThreaded[j][w]);
            }
            valueCount++;
        }


    }

}


/*
 * Test a task composition ( create 16 tasks and wait for results from 4 of them, resubmit these again with a different lambda and waiting again for results )
 *
 */
/*
TEST_CASE("Executor_TestComposition")
{

    for(int z=0;z<100;z++)
    {

        constexpr size_t bufferCount = 16;
        constexpr size_t taskGroupA = 4;
        constexpr size_t bufferSize = 256 * 8;

        constexpr float initialValue = 5.0f;

        FloatBufferArray buffers[bufferCount];

        TaskQueue *mainQueue = new TaskQueue();
        TaskQueue *responseQueueA = new TaskQueue();
        TaskQueue *responseQueueB = new TaskQueue();
        ExecutorGroup *group = new ExecutorGroup(ExecutorGroupId(0), Executor::hardwareConcurency() + 1, mainQueue);


        for (int i = 0; i < bufferCount; i++)
        {
            for (size_t sb = 0; sb < bufferSize; sb++)
            {
                buffers[i].push_back(initialValue);
            }
        }

        group->start();


        for (size_t j = 0; j < bufferCount; j++)
        {
            FloatBufferArray *buffer = &(buffers[j]);
            TaskQueue *queue = responseQueueA;
            if (j < taskGroupA)
            {
                queue = responseQueueB;
            }
            mainQueue->post(Task(TaskId(j + 1),
                                 []
                                         (const Task &task, InterruptPoint interruptPoint)
                                 {
                                     FloatBufferArray *array = (FloatBufferArray *) task.data();
                                     for (auto &value:*array)
                                     {
                                         value = value + 1;
                                     }
                                     return TaskResult::Success;
                                 }, buffer, queue));
        }

        size_t responseCount = taskGroupA;
        while (responseCount)
        {
            Task task = responseQueueB->get(true);
            CHECK(task.result().result == TaskResult::Success);
            responseCount--;

            FloatBufferArray *array = (FloatBufferArray *) task.data();
            for (auto value:*array)
            {
                CHECK(MathUtils::AlmostEqual(value, initialValue + 1));
            }

        }

        for (size_t j = 0; j < taskGroupA; j++)
        {
            FloatBufferArray *buffer = &(buffers[j]);
            mainQueue->post(Task(TaskId(j + 1),
                                 []
                                         (const Task &task, InterruptPoint interruptPoint)
                                 {
                                     FloatBufferArray *array = (FloatBufferArray *) task.data();
                                     for (auto &value:*array)
                                     {
                                         value = value - 1;
                                     }
                                     return TaskResult::Success;
                                 }, buffer, responseQueueB));
        }

        responseCount = taskGroupA;
        while (responseCount)
        {
            Task task = responseQueueB->get(true);
            CHECK(task.result().result == TaskResult::Success);
            responseCount--;
        }

        for (size_t j = 0; j < bufferCount; j++)
        {
            float valueToCheckAgainst = initialValue + 1;
            if (j < taskGroupA)
                valueToCheckAgainst = initialValue;

            for (size_t w = 0; w < bufferSize; w++)
            {
                CHECK(MathUtils::AlmostEqual(buffers[j][w], valueToCheckAgainst));
            }
        }


        group->stop();
        delete group;
        delete mainQueue;
        delete responseQueueA;
        delete responseQueueB;

    }
}
*/



/*
 * Test interrupting a never ending task ( basically thru cooperative interruption callback, this WILL not work for a task stuck in a some kind of wait state until it gets unstuck )
 *
 */


/*
TEST_CASE("Executor_TestInterruption")
{

    for(int z=0;z<100;z++)
    {

        constexpr size_t taskCount = 4;
        constexpr size_t mainGroupIterCount = 10000;

        for (int i = 0; i < 1000; i++)
        {

            TaskQueue *mainQueue = new TaskQueue();
            ExecutorGroup *group = new ExecutorGroup(ExecutorGroupId(0), Executor::hardwareConcurency() + 1, mainQueue);

            TaskQueue *responseQueue = new TaskQueue();

            int data[taskCount] = {0, 0, 0, 0};

            group->start();


            for (size_t j = 0; j < taskCount; j++)
            {
                if (j == 0) // never ending task
                {
                    mainQueue->post(Task(TaskId(j + 1),
                                         []
                                                 (const Task &task, InterruptPoint interruptPoint)
                                         {
                                             int *data = (int *) task.data();
                                             while (true)
                                             {
                                                 //StopTimer::sleep(10);
                                                 (*data)++;
                                                 if (interruptPoint.shouldStop())
                                                     return TaskResult::Skipped;
                                             }
                                             return TaskResult::Success;
                                         }, &data[j], responseQueue));
                }
                else
                {
                    mainQueue->post(Task(TaskId(j + 1),
                                         [mainGroupIterCount]
                                                 (const Task &task, InterruptPoint interruptPoint)
                                         {
                                             int *data = (int *) task.data();
                                             for (size_t i = 0; i < mainGroupIterCount; i++)
                                             {
                                                 (*data)++;
                                             }
                                             return TaskResult::Success;
                                         }, &data[j], responseQueue));
                }

            }


            size_t responseCount = taskCount - 1;
            while (responseCount)
            {
                Task task = responseQueue->get(true);
                CHECK(task.result().result == TaskResult::Success);
                responseCount--;
            }

            group->stop();

            Task task = responseQueue->get(true);
            CHECK(task.id() == 1);
            CHECK(task.result().result == TaskResult::Skipped);

            for (size_t j = 1; j < taskCount; j++)
            {
                CHECK(data[j] == mainGroupIterCount);
            }


            delete group;
            delete mainQueue;
            delete responseQueue;

        }
    }

}
*/
