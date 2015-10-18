

#include "RenderResource.h"
#include "RenderManager.h"
#include "RenderResourceId.h"

using namespace MiniEngine;





Resource::Resource(RenderManager *manager):mManagerProxy(manager)
{

}



RenderResource::RenderResource(RenderManager *manager):Resource(manager)
{

}

RenderResource::~RenderResource()
{
    handler().releaseResource(mId[0]);
    handler().releaseResource(mId[1]);
}


RenderResourceIdBuffer RenderResource::memoryBuffer(RenderBufferSource source)
{
    if(manager())
        return manager()->memoryBuffer(id(source));
    return RenderResourceIdBuffer();
}
