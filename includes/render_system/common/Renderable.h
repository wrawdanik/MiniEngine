#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "RenderResource.h"
#include "GeometryUtils.h"
#include "DataTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace MiniEngine
{
    
    
    class Renderable:public Resource
    {
    public:
        
        Renderable(RenderManager *manager);
        virtual ~Renderable()=0;
        
        
        inline VertexBufferPtr vertexBuffer() { return mVBuffer;}
        inline IndexBufferPtr indexBuffer() { return mIBuffer;}
        inline const AABoundingBox&	boundingBox()  const { return mBBox;};
        
    protected:
        
        
        inline void setBoundingBox(const AABoundingBox &box)
        {
            mBBox=box;
        }

        StatusPtr createRenderable(const VertexBufferDescriptor &vertexDesc,const IndexBufferDescriptor &indexDesc=IndexBufferDescriptor());
                
        
    private:
        VertexBufferPtr mVBuffer;
        IndexBufferPtr mIBuffer;
        AABoundingBox	mBBox;
    };
    
    using RenderableArray=rde::vector<Renderable*>;
    using RenderablePtr=intrusive_ptr<Renderable>;
    using RenderablePtrArray=rde::vector<RenderablePtr>;
    
}

#endif