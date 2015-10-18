//
// Created by Walter Rawdanik on 10/10/15.
//

#ifndef MINIENGINE_RENDERBACKENDMANAGER_MEMORY_TEST_H
#define MINIENGINE_RENDERBACKENDMANAGER_MEMORY_TEST_H


#include "RenderResourceId.h"
#include "Status.h"
#include "Platform.h"
#include "ObjectPool.h"


namespace MiniEngine
{

    class ResourceRequest;
    class TaskQueue;

    struct BackendMemoryBufferSegment
    {
        enum class SegmentStatus
        {
            Unlocked,
            LockedUser,
            LockedRender
        };

        BackendMemoryBufferSegment()
        {
            segment= nullptr;
            status=SegmentStatus::Unlocked;
        }

        BackendMemoryBufferSegment(unsigned char* segment)
        {
            this->segment=segment;
            status=SegmentStatus::Unlocked;
        }



        unsigned char* segment;
        SegmentStatus    status;
    };

    using BackendMemoryBufferSegmentArray=rde::vector<BackendMemoryBufferSegment>;

    struct BackendMemoryBuffer
    {
        BackendMemoryBuffer()
        {
            buffer= nullptr;
            sizeInBytes=0;
            unlocked=&segments[0];
        }

        BackendMemoryBuffer(size_t sizeInBytes, size_t instanceCount);

        ~BackendMemoryBuffer();

        unsigned char* buffer;
        size_t sizeInBytes;
        BackendMemoryBufferSegment *unlocked;
        BackendMemoryBufferSegmentArray segments;
    };

    class RenderBackendManagerMemoryTest
    {


    public:

        RenderBackendManagerMemoryTest();



        ~RenderBackendManagerMemoryTest();


        void allocateResource(ResourceRequest *request);
        void releaseResource(ResourceRequest *request);


    private:

        using  BackedRenderResourceIdBufferMap=rde::hash_map<size_t,BackendMemoryBuffer*>;

        ObjectPool<BackendMemoryBuffer>  mBackedBufferPool;
        BackedRenderResourceIdBufferMap mResourceMap;




    };
}


#endif //MINIENGINE_RENDERBACKENDMANAGER_MEMORY_TEST_H
