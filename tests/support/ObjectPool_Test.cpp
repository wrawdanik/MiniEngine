
#define CATCH_CONFIG_MAIN 
#define DEBUG

#define CATCH_CONFIG_CPP11_NULLPTR

#include "catch.hpp"
#include "ObjectPool.h"
#include "Node2d.h"
#include "Range.h"

using namespace MiniEngine;
using namespace std;


TEST_CASE("ObjectPool_DefaultSize")
{
    ObjectPool<Node2d> pool;
    
	CHECK(pool.availableCount()==16);
    CHECK(pool.allocationCount()==1);

    Range<size_t> range1(10,5);

    size_t offset=5;

    Range<size_t> range2=Range<size_t>(range1,12);

    using RangeInt=Range<size_t>;

    rde::vector<RangeInt> ranges;

    Random rand;

    for(int i=0;i<20;i++)
    {
        ranges.push_back(RangeInt(rand.rand(10,100),1));
    }

    rde::quick_sort(ranges.begin(),ranges.end());

    int bla=20;


}

TEST_CASE("ObjectPool_InvalidAllocation")
{
    size_t poolSize=4;
    size_t maxPoolSize=5;
    
    ObjectPool<Node2d> pool(poolSize,maxPoolSize);
    
    Node2dArray nodes;
    
    for(int i=0;i<maxPoolSize;i++)
    {
        Node2d *node=pool.create();
    }

    CHECK(pool.allocationCount()==2);

    Node2d *node=pool.create();
    CHECK(node == nullptr);
    
}

TEST_CASE("ObjectPool_AllocationRelease")
{
    ObjectPool<Node2d> pool(8,8);

    Node2dArray nodes;
    
    for(int i=0;i<pool.capacity();i++)
    {
        Node2d *node=pool.create();
        CHECK(node!= nullptr);
        nodes.push_back(node);
    }
    
    CHECK(nodes.size() == pool.capacity());
    CHECK(0 == pool.availableCount());
    
    Node2dArray::iterator curr=nodes.begin();
    
    while(curr!=nodes.end())
    {
        pool.destroy(*curr);
        ++curr;
    }
    nodes.clear();
    
    CHECK(pool.capacity() == pool.availableCount());
}
 
TEST_CASE("ObjectPool_MultipleRelease")
{
    size_t poolSize=32;
    
    ObjectPool<Node2d> pool(poolSize,poolSize);
    
	CHECK(pool.capacity()==poolSize);
    
    Node2dArray nodes;
    
    for(int i=0;i<poolSize;i++)
    {
        Node2d *node=pool.create();
        CHECK(node!=nullptr);
        nodes.push_back(node);
    }
    
    CHECK(nodes.size() == pool.capacity());
    CHECK(0 == pool.availableCount());
    
    pool.destroy(nodes[0], nodes.size());
    nodes.clear();
    
    CHECK(pool.capacity() == pool.availableCount());
}

TEST_CASE("ObjectPool_ItemReset")
{
    size_t poolSize=12;
    
    ObjectPool<Node2d> pool(poolSize);
    
    Node2dArray nodes;
    
    Node2d *child=0;
    
    for(int i=0;i<poolSize;i++)
    {
        Node2d *node=pool.create();
        CHECK(node!=nullptr);
        if(child!= nullptr)
            node->addChild(child);
        child=node;
        nodes.push_back(node);
    }
    pool.destroy(nodes[0], nodes.size());
    nodes.clear();

    for(int i=0;i<poolSize;i++)
    {
        Node2d *node=pool.create();
        if(node->children())
            CHECK(node->children()->empty());
    }
}

TEST_CASE("ObjectPool_Reset") 
{
    size_t poolSize=12;
    size_t maxPoolSize=13;
    
    ObjectPool<Node2d> pool(poolSize,maxPoolSize);
    for(int i=0;i<poolSize;i++)
    {
        Node2d *node=pool.create();
    }
    
    CHECK(0 == pool.availableCount());
    
    pool.reset();

    CHECK(poolSize == pool.availableCount());
    
}

TEST_CASE("ObjectPool_CheckSequential")
{

    size_t poolSize=12;
    size_t maxPoolSize=13;
    
    Node2d *prevNode=0;
    
    ObjectPool<Node2d> pool(poolSize,maxPoolSize);
    for(int i=0;i<poolSize;i++)
    {
        Node2d *node=pool.create();
        if(prevNode!=0)
        {
            CHECK((prevNode+1)==node);
        }
        prevNode=node;
    }

    Node2d *node=pool.create();
	REQUIRE(node!=nullptr);

    CHECK(pool.allocationCount()==2);
    
}


TEST_CASE("ObjectPool_MultipleAllocations")
{

    size_t poolSize=12;
    size_t maxPoolSize=26;

    Node2d *prevNode=0;

    ObjectPool<Node2d> pool(poolSize,maxPoolSize,256);
    for(int i=0;i<maxPoolSize;i++)
    {
        Node2d *node=pool.create();
		REQUIRE(node!=nullptr);
    }

    CHECK(pool.availableCount()==0);
    CHECK(pool.allocationCount()==2);
}

TEST_CASE("ObjectPool_MultipleAllocationsBy1")
{

    size_t poolSize=1;
    size_t maxPoolSize=513;

    Node2d *prevNode=0;

    ObjectPool<Node2d> pool(poolSize,maxPoolSize,1);
    for(int i=0;i<maxPoolSize;i++)
    {
        Node2d *node=pool.create();
        CHECK(node!=nullptr);
    }

    CHECK(pool.availableCount()==0);
    CHECK(pool.allocationCount()==513);

    pool.reset();

    CHECK(pool.availableCount()==513);
    CHECK(pool.allocationCount()==513);

}