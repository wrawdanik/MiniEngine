#include "IndexBuffer.h"
#include "RenderManager.h"


using namespace MiniEngine;

IndexBuffer::IndexBuffer(RenderManager *manager,const IndexBufferDescriptor &descriptor): RenderResource(manager),mStrips(descriptor.strips),mRange(descriptor.range)
{
    mStride=descriptor.stride;
    mMode=descriptor.mode;
    setValid(allocateResources(descriptor));

    _LogDebugAssert(isValid(),"IndexBuffer initialization failure.");
}

IndexBuffer::~IndexBuffer()
{

}

bool IndexBuffer::allocateResources(const IndexBufferDescriptor & descriptor)
{
    if(!descriptor.isValid() || manager()== nullptr)
        return false;

    if(descriptor.clientSide)
    {
        mResourceId[static_cast<size_t>(RenderBufferSource::Client)]=manager()->allocateResource(ResourceType::MemoryBuffer,descriptor.mode,descriptor.sizeInBytes());
        if(!mResourceId[static_cast<size_t>(RenderBufferSource::Client)].isValid())
            return false;
    }
    mResourceId[static_cast<size_t>(RenderBufferSource::Server)]=manager()->allocateResource(ResourceType::MemoryBuffer,descriptor.mode,descriptor.sizeInBytes());
    return mResourceId[static_cast<size_t>(RenderBufferSource::Server)].isValid();
}

RenderResourceIdBuffer IndexBuffer::memoryBuffer(RenderBufferSource source)
{
    if(manager()!= nullptr)
    {
        return manager()->memoryBuffer(mResourceId[static_cast<size_t>(source)]);
    }
    return RenderResourceIdBuffer();
}

/*
BufferContentTaskFuture IndexBuffer::submitBufferContentTask(const BufferContentTask &task,RenderBufferSource source)
{
    if(manager()!= nullptr)
    {
        return manager()->submitBufferContentTask(task,id(source));
    }

    return BufferContentTaskFuture();
}
 */