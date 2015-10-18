#include <memory>
#include "rdestl.h"

void testFunctionRdeVector();
void testFunctionStlVector();


void testFunctionRdeMap();
void testFunctionStlMap();


void testFunctionRdeUpperBound();
void testFunctionStlUpperBound();


void testFunctionRdeQuickSort();
void testFunctionStlQuickSort();

void testSharedPointer();

void incrementPointer1(float *val);

void incrementPointer1(std::shared_ptr<float> val);


struct TestSortData
{
    TestSortData(void *val,float distance)
    {
        billboard=val;
        distanceSquared=distance;
    }

    void *billboard;
    float	   distanceSquared;
};

struct TestSort
{
    bool operator()(const TestSortData  &lhs, const TestSortData &rhs) const
    {
        return lhs.distanceSquared > rhs.distanceSquared;
    }
};
