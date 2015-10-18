#include "VertexBuffer.h"
#include "rdestl.h"
#include "fixed_vector.h"
#include "Platform.h"

using namespace MiniEngine;




VertexBuffer::VertexBuffer(RenderManager *manager,const VertexBufferDescriptor & descriptor): Resource(manager),mRange(descriptor.range)
{
    mRenderPrimitive=descriptor.renderPrimitive;
    setValid(allocateResources(descriptor));

    _LogDebugAssert(isValid(),"VertexBuffer initialization failure.");
}

VertexBuffer::~VertexBuffer()
{

}

bool VertexBuffer::allocateResources(const VertexBufferDescriptor & descriptor)
{

    if(!descriptor.isValid() || manager()== nullptr)
        return false;

    mRenderPrimitive=descriptor.renderPrimitive;

    mSizeInBytes=0;

    using VertexAttributeDataFixedArray=rde::fixed_vector<VertexAttributeData,32,true>;

    VertexAttributeDataFixedArray shadowedStatic;
    VertexAttributeDataFixedArray shadowedDynamic;
    VertexAttributeDataFixedArray staticAtt;
    VertexAttributeDataFixedArray dynamicAtt;

    for(auto attData:descriptor.attributes)
    {
        mSizeInBytes+=attData.sizeInBytes();
        if (attData.renderMode() == RenderBufferMode::Dynamic)
        {
            dynamicAtt.push_back(attData);
            if(attData.clientSide())
                shadowedDynamic.push_back(attData);
        }
        else
        {
            staticAtt.push_back(attData);
            if(attData.clientSide())
                shadowedStatic.push_back(attData);
        }
    }

    if(!dynamicAtt.empty())
    {
        mDynamicAttributes = new VertexAttributesList(manager(), &dynamicAtt[0], dynamicAtt.size());
        mDynamicAttributes->setId(manager()->allocateResource(ResourceType::MemoryBuffer, RenderBufferMode::Dynamic, descriptor.range.size * mDynamicAttributes->sizeInBytes()),
                                  RenderBufferSource::Server);
        if (!mDynamicAttributes->id(RenderBufferSource::Server).isValid())
            return false;
        if (!shadowedDynamic.empty())
        {
        mDynamicAttributes->setId(manager()->allocateResource(ResourceType::MemoryBuffer, RenderBufferMode::Dynamic, descriptor.range.size *
                                                                                                mDynamicAttributes->sizeInBytes(), RenderBufferSource::Client, 1),
                                  RenderBufferSource::Client);
            if (!mDynamicAttributes->id(RenderBufferSource::Client).isValid())
                return false;
        }
    }

    if(!staticAtt.empty())
    {
        mStaticAttributes=new VertexAttributesList(manager(),&staticAtt[0],staticAtt.size());
        mStaticAttributes->setId(manager()->allocateResource(ResourceType::MemoryBuffer,RenderBufferMode::Static,descriptor.range.size*mStaticAttributes->sizeInBytes(),RenderBufferSource::Server,1),
        RenderBufferSource::Server);
        if(!mStaticAttributes->id(RenderBufferSource::Server).isValid())
            return false;
        if (!shadowedStatic.empty())
        {
            mStaticAttributes->setId(manager()->allocateResource(ResourceType::MemoryBuffer, RenderBufferMode::Static, descriptor.range.size *
                                             mStaticAttributes->sizeInBytes(), RenderBufferSource::Client, 1),
                                      RenderBufferSource::Client);
            if (!mStaticAttributes->id(RenderBufferSource::Client).isValid())
                return false;
        }
    }


    return true;
}
