#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "RenderResourceId.h"
#include "VertexAttributes.h"
#include "RenderResource.h"
#include "Range.h"

namespace MiniEngine
{
    class RenderManager;


    
    class VertexBufferDescriptor
    {

        public:
        VertexBufferDescriptor()
        {
            renderPrimitive=RenderPrimitive::Triangle;
        }

        VertexBufferDescriptor(const VertexAttributeDataArray &vArray,const BufferItemRange &range, RenderPrimitive primitive):attributes(vArray),range(range)
        {
            renderPrimitive=primitive;
        }

        inline bool isValid() const
        {
            return (!range.isEmpty() && !attributes.empty());
        }

        VertexAttributeDataArray attributes;
        BufferItemRange range;
        RenderPrimitive  renderPrimitive;
    private:

    };


    
    class VertexBuffer: public Resource
    {
    public:
        
        VertexBuffer(RenderManager *manager,const VertexBufferDescriptor & descriptor);
        ~VertexBuffer();
        
        inline size_t sizeInBytes() const { return mSizeInBytes;}
        VertexAttributesListPtr dynamicAttributes() const { return mDynamicAttributes;}
        VertexAttributesListPtr staticAttributes() const { return mDynamicAttributes;}
        RenderPrimitive  renderPrimitive() const { return mRenderPrimitive;}
        BufferItemRange range() const { return mRange;}

    private:

        bool allocateResources(const VertexBufferDescriptor & descriptor);


        VertexAttributesListPtr mStaticAttributes;
        VertexAttributesListPtr mDynamicAttributes;
        RenderPrimitive  mRenderPrimitive;
        size_t mSizeInBytes;
        BufferItemRange mRange;
    };

    using    VertexBufferPtr=intrusive_ptr<VertexBuffer>;
    typedef  rde::vector<VertexBufferPtr> VertexBufferPtrArray;
}
#endif