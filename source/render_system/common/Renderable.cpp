#include "Renderable.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderManager.h"

using namespace MiniEngine;

Renderable::Renderable(RenderManager *manager):Resource(manager)
{
    
}




StatusPtr Renderable::createRenderable(const VertexBufferDescriptor &vertexDesc,const IndexBufferDescriptor &indexDesc)
{
    StatusPtr status;

    if(vertexDesc.isValid())
    {
        mVBuffer=new VertexBuffer(manager(),vertexDesc);
        if(!mVBuffer->isValid())
            return mVBuffer->status();
    }
    if(indexDesc.isValid())
    {
        mIBuffer = new IndexBuffer(manager(), indexDesc);
        if(!mIBuffer->isValid())
            return mIBuffer->status();
    }
    return status;
}
