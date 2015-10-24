#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "RenderResourceId.h"
#include "rdestl.h"
#include "VertexAttributes.h"
#include "RenderResource.h"

namespace MiniEngine
{
    class RenderManager;

    using StripArray=rde::vector<uint32_t>;

    class IndexBufferDescriptor
    {
    public:
        IndexBufferDescriptor()
        {
            stride=0;
            mode=RenderBufferMode::Static;
            clientSide=false;
        }

        IndexBufferDescriptor(const BufferItemRange &iRange,RenderData iType=RenderData::UShort,RenderBufferMode mode=RenderBufferMode::Static):range(iRange),
                                                                                                                                               indexType(iType),
                                                                                                                                               mode(mode)
        {
            stride = DataTypes::sizeInBytes(indexType);
        }


        inline bool isValid() const
        {
            return (!range.isEmpty() && stride!=0);
        }
        
        inline size_t sizeInBytes() const
        {
            return DataTypes::sizeInBytes(indexType)*range.size;
        }

        BufferItemRange range;
        size_t        stride;
        rde::vector<unsigned int>	strips;
        RenderBufferMode mode;
        RenderData indexType;
        bool clientSide;
    };
    


    class IndexBuffer:public RenderResource
    {
    public:
        IndexBuffer(RenderManager *manager,const IndexBufferDescriptor &descriptor);
        ~IndexBuffer();
        
        inline size_t sizeInBytes() const {  return mRange.size*mStride;}
        //void populate(size_t indexOffset,size_t indexCount,size_t stride, BufferId indexData, const unsigned int* strips,size_t stripeCount,RenderBufferMode::Type renderMode);


        RenderResourceIdBuffer memoryBuffer(RenderBufferSource source);
        inline RenderResourceId resourceId(RenderBufferSource source) const  { return mResourceId[static_cast<size_t>(source)];}
        inline const StripArray& strips() const { return mStrips;}
        inline size_t stride() const { return mStride;}

        inline BufferItemRange range() const { return mRange;}


        //BufferContentTaskFuture submitBufferContentTask(const BufferContentTask &task, RenderBufferSource source=RenderBufferSource::Server);

    private:

        bool allocateResources(const IndexBufferDescriptor & descriptor);

        size_t      mStride;
        RenderResourceId    mResourceId[2];
        StripArray	mStrips;
        RenderBufferMode mMode;
        BufferItemRange mRange;

    };
    
    typedef   intrusive_ptr<IndexBuffer> IndexBufferPtr;
    typedef  rde::vector<IndexBufferPtr> IndexBufferPtrArray;
}
#endif