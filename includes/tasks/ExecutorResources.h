//
// Created by Walter Rawdanik on 9/25/15.
//

#ifndef MINIENGINE_EXECUTORRESOURCES_H
#define MINIENGINE_EXECUTORRESOURCES_H

#include "inttypes.h"

#include <atomic>

class ExecutorGroupId
{
public:

    explicit ExecutorGroupId( uint32_t val):value(val){}

    ExecutorGroupId():value(0) {}

    inline void reset()
    {
        value=0;
    }

    inline bool isValid() const
    {
        return value!=0;
    }
    uint32_t value;
};

class ExecutorId
{
public:


    explicit ExecutorId( uint32_t val, ExecutorGroupId groupId=ExecutorGroupId()):value((((uint64_t) val)&0xFFFFFFFF)|(((uint64_t)groupId.value)<<32))
    {
        int bla=20;
    }

    ExecutorId():value(0) {}

    inline ExecutorGroupId groupId()
    {
        return ExecutorGroupId(value>>32);
    }

    inline operator uint64_t () const
    {
        return (uint64_t)value;
    }

    inline void reset()
    {
        value=0;
    }

    inline bool isValid() const
    {
        return value!=0;
    }
    uint64_t value;
};

class TaskId
{
public:

    explicit TaskId( uint32_t val):value(val){}
    TaskId():value(0) {}

    inline bool isValid() const
    {
        return value!=0;
    }

    inline void reset()
    {
        value=0;
    }

    inline operator unsigned int () const
    {
        return (unsigned int)value;
    }

    uint32_t value;
};



class InterruptPoint
{
public:

    InterruptPoint()
    {
        mFlag= nullptr;
    }

    InterruptPoint(std::atomic_flag *flag):mFlag(flag)
    {

    }

    inline bool shouldStop()
    {
        if(mFlag)
            return (!mFlag->test_and_set());
        return false;
    }

private:

    std::atomic_flag *mFlag;
};

#endif //MINIENGINE_EXECUTORRESOURCES_H
