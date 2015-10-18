//
// Created by Walter Rawdanik on 10/10/15.
//

#include <RenderTask.h>
#include "RenderBackendManagerMemoryTest.h"
#include "RenderTask.h"
#include "Log.h"

using namespace MiniEngine;

BackendMemoryBuffer::BackendMemoryBuffer(size_t sizeInBytes, size_t instanceCount)
{
    buffer=new unsigned char[instanceCount*sizeInBytes];

    unsigned char *data=buffer;

    for(size_t i=0;i<instanceCount;i++)
    {
        segments.push_back(BackendMemoryBufferSegment(data));
        data+=sizeInBytes;
    }

}

BackendMemoryBuffer::~BackendMemoryBuffer()
{


}


RenderBackendManagerMemoryTest::RenderBackendManagerMemoryTest()
{
    mResourceMap.reserve(512);
}


RenderBackendManagerMemoryTest::~RenderBackendManagerMemoryTest()
{

#ifdef DEBUG

    if(!mResourceMap.empty())
    {
        _LogDebug("RenderBackendManagerMemoryTest:: on exit still found some allocated resources (%d) - release these.",mResourceMap.size());
    }
#endif
    for(auto backendVal:mResourceMap)
    {
        delete backendVal.second;
    }
}


void RenderBackendManagerMemoryTest::allocateResource(ResourceRequest *request)
{
    if(request!=nullptr)
    {
        request->mData.status=static_cast<size_t>(Status::Code::Success);

        if(request->mResourceId.isValid() && request->mData.size>0 && request->mData.instances>0)
        {
            BackendMemoryBuffer *memoryBuffer=new (std::nothrow) BackendMemoryBuffer(request->mData.size,request->mData.instances);
            if(memoryBuffer!= nullptr)
            {
                mResourceMap.insert(rde::make_pair(request->mResourceId.uid(),memoryBuffer));
                if(static_cast<RenderBufferMode>(request->mData.mode)!=RenderBufferMode::Static)
                {
                    request->mBuffer=memoryBuffer->buffer;
                }
            }
            else
            {
                request->setStatus(Status::Code::OutOfMemory);
            }
        }
        else
        {
            request->setStatus(Status::Code::InvalidParameter);
        }




        _LogDebugAssert((!request->status()==Status::Code::Success),"Operation RenderBackendManagerMemoryTest::allocateResource failed with code %d",
        static_cast<size_t>(request->status()));
    }
}

void RenderBackendManagerMemoryTest::releaseResource(ResourceRequest *request)
{
    if(request!=nullptr)
    {
        request->setStatus(Status::Code::InvalidParameter);

        if(request->mResourceId.isValid())
        {
            auto found=mResourceMap.find(request->mResourceId.uid());
            if(found!=mResourceMap.end())
            {
                delete found->second;
                mResourceMap.erase(found);
                request->setStatus(Status::Code::Success);
            }
        }
    }

}