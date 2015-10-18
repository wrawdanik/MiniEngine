


#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <limits>
#include <assert.h>
#include "Log.h"

namespace MiniEngine
{

template <class T>
class	ObjectPool
{
public:

    ObjectPool(size_t initialCapacity=16,size_t maxItems=std::numeric_limits<size_t>::max(), size_t increaseStep=0)
    {
        mAllocationCount=0;
        mCurrItems=0;
        mMaxItems=maxItems;
        mIncreaseStep=increaseStep;
        if(mIncreaseStep==0)
            mIncreaseStep=(initialCapacity>>1);
        resize(initialCapacity);
    }

    ObjectPool &operator=(const ObjectPool &other) = delete;
    ObjectPool(const ObjectPool &other) = delete;

    ~ObjectPool()
    {
        for(auto &cacheItem:mItemCaches)
        {
            delete [] cacheItem.cache;
        }
    }


        
    T* create()
    {

        if(!mFreeItems.empty())
        {
            T *item=mFreeItems.back();
            mFreeItems.pop_back();
            item->reset();		
            return item;
        }
        else
        {
            if(resize(mIncreaseStep))
            {
#ifdef DEBUG
                if(mFreeItems.empty())
                {
                    _LogDebug("ObjectPool::create : Successfully resized by %d items.",mIncreaseStep);
                }
#endif
                T *item=mFreeItems.back();
                mFreeItems.pop_back();
                item->reset();
                return item;
            }
            _LogDebug("ObjectPool::create : Failed to allocate new item. Max capacity (%d) reached.",mMaxItems);
            return nullptr;
        }
    }
    bool destroy(T *item)
    {
        if(item==nullptr)
            return false;

#ifdef DEBUG
        for(auto &cacheItem:mItemCaches)
        {
           assert(item>=cacheItem.cache && item<(cacheItem.cache+cacheItem.count));
        }
#endif
        mFreeItems.push_back(item);
        return true;
    }
    
    size_t destroy(T *items, size_t count)
    {
        size_t itemCount=0;
        
        T *current=items;
        T *last=items+count;
        
        while(current!=last)
        {
            T *item=current;
            if(item!=nullptr)
            {

#ifdef DEBUG
                for(auto &cacheItem:mItemCaches)
                {
                    assert(item>=cacheItem.cache && item<(cacheItem.cache+cacheItem.count));
                }
#endif
                mFreeItems.push_back(item);
            }
            ++itemCount;
            ++current;
        }
        return itemCount;        
    }
    size_t capacity() const { return mMaxItems;}
    size_t availableCount() const { return mFreeItems.size();}
    size_t allocationCount() const { return mAllocationCount;}


    void reset()
    {
        mFreeItems.clear();

        for(auto &cacheItem:mItemCaches)
        {
            T *cache=cacheItem.cache+cacheItem.count;
            for( size_t i=0;i<cacheItem.count;i++)
            {
                mFreeItems.push_back(--cache);
            }
        }
    }
    
private:





    bool resize(size_t count)
    {
        if(mCurrItems+count>mMaxItems)
            count=mMaxItems-mCurrItems;

        if(count>0 && mCurrItems+count<=mMaxItems)
        {
            T* cache=new T[count];
            mItemCaches.push_back(CacheItem(count,cache));
            mAllocationCount++;
            cache+=count;
            for( size_t i=0;i<count;i++)
            {
                mFreeItems.push_back(--cache);
            }
            mCurrItems+=count;
            return true;
        }
        return false;
    }

    struct CacheItem
    {
        CacheItem(size_t count, T * cache)
        {
            this->count=count;
            this->cache=cache;
        }

        size_t count;
        T   *cache;
    };

    typedef rde::vector<T*>	ItemPtrArray;
    typedef rde::vector<CacheItem>	CacheItemItemPtrArray;

    ItemPtrArray	mFreeItems;
    CacheItemItemPtrArray    mItemCaches;
    size_t mMaxItems;
    size_t mCurrItems;
    size_t mIncreaseStep;
    size_t mAllocationCount;
	
};
    
}

#endif