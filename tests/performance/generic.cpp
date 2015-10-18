#define CATCH_CONFIG_MAIN



#include "generic.h"
#include "StopTimer.h"

#include "Log.h"
#include "radix_sorter.h"

#include <vector>
#include <unordered_map>

#include "random.h"

using namespace MiniEngine;


int main ()
{
    /*
    testFunctionRdeVector();
    testFunctionStlVector();


    testFunctionRdeMap();
    testFunctionStlMap();


    testFunctionRdeUpperBound();
    testFunctionStlUpperBound();

*/


    //testFunctionRdeQuickSort();
    //testFunctionStlQuickSort();



    testSharedPointer();

}



void testFunctionRdeVector()
{
    rde::vector<float> values;
    size_t size=5000;
    values.reserve(size);

    StopTimer stopTimer;
    float total=0;

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.push_back((float)j);
        }

        for(float val : values)
            total+=val;

    }


    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();


    _Log("Total is %f.  rde::vector<float> took %lu milliseconds \n",total,millisElapsed);
}

void testFunctionStlVector()
{

    std::vector<float> values;

    size_t size=5000;
    values.reserve(size);

    StopTimer stopTimer;
    float total=0;

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.push_back((float)j);
        }

        for(float val : values)
            total+=val;

    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("Total is %f.  std::vector<float> took %lu milliseconds \n",total,millisElapsed);
}

void testFunctionRdeMap()
{
    size_t size=1000;

    rde::hash_map<long,float>  values;
    values.reserve(size);

    StopTimer stopTimer;
    float total=0;

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.insert(rde::make_pair((long)j,(float)j));
        }

        for(int j=0;j<size;j++)
        {
            total+=values[j];
        }
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("Total is %f.  rde::hash_map<long,float> took %lu milliseconds \n",total,millisElapsed);

}
void testFunctionStlMap()
{
    size_t size=1000;

    std::unordered_map<long, float> values;

    values.reserve(size);

    StopTimer stopTimer;
    float total=0;

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.insert(std::make_pair((long)j,(float)j));
        }

        for(int j=0;j<size;j++)
        {
            total+=values[j];
        }
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("Total is %f.  std::unordered_map<long, float> took %lu milliseconds \n",total,millisElapsed);

}

void testFunctionRdeUpperBound()
{
    rde::vector<int> values;
    size_t size=10000;
    values.reserve(size);

    StopTimer stopTimer;
    int total=0;

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.push_back((int)j);
        }

        auto found=rde::lower_bound(values.begin(),values.end(),3997,rde::less<int>());
        total+=*found;
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("Total is %d.  rde::lower_bound rde::vector<int> took %lu milliseconds \n",total,millisElapsed);

}
void testFunctionStlUpperBound()
{
    std::vector<int> values;
    size_t size=10000;
    values.reserve(size);

    StopTimer stopTimer;
    int total=0;

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.push_back((int)j);
        }

        auto found=std::lower_bound(values.begin(),values.end(),3997,std::less<int>());
        total+=*found;
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("Total is %d.  std::lower_bound std::vector<int> took %lu milliseconds \n",total,millisElapsed);
}



void testFunctionRdeQuickSort()
{
    rde::vector<TestSortData> values;
    size_t size=2000;
    values.reserve(size);

    StopTimer stopTimer;
    int total=0;

    Random rnd;

    rde::vector<float> floatValues;
    floatValues.reserve(size);

    for(int j=0;j<size;j++)
    {
        floatValues.push_back(rnd.randf());
    }

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.push_back(TestSortData(nullptr,floatValues[j]));
        }

        TestSort bSort;
        rde::quick_sort(values.begin(),values.end(),bSort);
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("  rde::quick_sort rde::vector<TestSortData> took %lu milliseconds \n",millisElapsed);
}

void testFunctionStlQuickSort()
{
    std::vector<TestSortData> values;
    size_t size=2000;
    values.reserve(size);

    StopTimer stopTimer;
    int total=0;

    Random rnd;

    rde::vector<float> floatValues;
    floatValues.reserve(size);

    for(int j=0;j<size;j++)
    {
        floatValues.push_back(rnd.randf());
    }

    for(int i=0;i<100000;i++)
    {
        values.clear();
        for(int j=0;j<size;j++)
        {
            values.push_back(TestSortData(nullptr,floatValues[j]));
        }

        TestSort bSort;

        std::sort(values.begin(),values.end(),bSort);
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();

    _Log("  std::sort std::vector<TestSortData> took %lu milliseconds \n",millisElapsed);
}



void testSharedPointer()
{
    auto buffer=std::make_shared<float>(1024);



    StopTimer stopTimer;

    for(size_t i=0;i<10000;i++)
    {
        for(size_t w=0;w<1024;w++)
        {
            incrementPointer1(buffer);

            if(buffer)
            {

                float *val=buffer.get();
                *val+=1;
            }
        }
    }

    uint64_t millisElapsed=stopTimer.elapsedTimeMilliseconds();
    _Log(" Passing around std::shared_ptr took %lu milliseconds \n",millisElapsed);

    stopTimer.restart();

    float *val=buffer.get();

    for(size_t i=0;i<10000;i++)
    {
        for(size_t w=0;w<1024;w++)
        {
            incrementPointer1(val);
        }
    }

    StopTimer::ElapsedTime eTime=stopTimer.elapsedTime();

    millisElapsed=stopTimer.elapsedTimeMilliseconds();
    _Log("  Passing around regular ptr took %lu,%lu milliseconds  and second %f\n",eTime.first,millisElapsed,eTime.second);

}

void incrementPointer1(float *val)
{
    if(val)
    {
        *val+=1;
    }
}

void incrementPointer1(std::shared_ptr<float> val)
{
    if(val)
    {
        float *value=val.get();
        *val+=1;
    }
}