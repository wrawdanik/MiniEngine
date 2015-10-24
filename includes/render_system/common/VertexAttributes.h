

#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include "rdestl.h"
#include "DataTypes.h"
#include "RenderResourceId.h"
#include "MiscUtils.h"
#include "Platform.h"
#include "RenderManager.h"
#include "RenderResource.h"


namespace MiniEngine
{
    class RenderManager;



	class VertexAttributeData
	{

	public:

		VertexAttributeData()
		{
            mData.value=0;
		}

        VertexAttributeData(VertexBinding binding,size_t elementCount,RenderData type,RenderBufferMode renderMode=RenderBufferMode::Static,bool normalized=false,bool clientSide=false,bool enabled=true)
        {
            mData.binding=static_cast<size_t>(binding);
            mData.elementCount=elementCount;
            mData.type=static_cast<size_t>(type);
            mData.renderMode=static_cast<size_t>(renderMode);
            mData.normalized=(normalized) ? 1 : 0;
            mData.clientSide=(clientSide) ? 1 : 0;
            mData.enabled=(enabled) ? 1 : 0;
            mData.elementSize=DataTypes::sizeInBytes(type);
        }
        
        inline size_t sizeInBytes(size_t alignmentBytes=RenderDefaults::vertexAlignment()) const
        {
            if(alignmentBytes==1)
                return mData.elementCount*mData.elementSize;
            return MiscUtils::alignedTo(alignmentBytes,mData.elementCount*mData.elementSize);
        }



        inline void         setEnabled(bool flag)  { mData.normalized=(flag) ? 1 : 0;}

        inline VertexBinding binding() const  { return static_cast<VertexBinding>(mData.binding); }
        inline RenderData    renderData() const  { return static_cast<RenderData>(mData.type); }
        inline RenderBufferMode    renderMode() const  { return static_cast<RenderBufferMode>(mData.renderMode); }
        inline size_t       elementCount() const  { return mData.elementCount; }
        inline size_t       elementSize() const  { return mData.elementSize; }
        inline bool         clientSide() const { return (mData.clientSide!=0);}
        inline bool         enabled() const { return (mData.enabled!=0);}
        inline bool         normalized() const { return (mData.normalized!=0);}





    private:

        union AttributeData
        {
            struct
            {
                size_t elementSize:     16;
                size_t elementCount:    16;
                size_t binding:         8;
                size_t type:            8;
                size_t renderMode:      2;
                size_t normalized:      1;
                size_t enabled:         1;
                size_t clientSide:      1;
            };
            uint64_t value;
        };
        AttributeData mData;
    };


    using VertexAttributeDataArray=rde::vector<VertexAttributeData>;


/** 
* @class VertexAttributesList
*
* @brief This class declares the format of a set of vertex data channels attached to the parent MiniEngine::VertexBuffer
*
* 	 
*/

    class VertexAttribute
    {
    public:

        VertexAttribute():mData(VertexAttributeData())
        {
            mOffset=0;
            mIndex=0;
            mStride=0;
        }


        VertexAttribute(const VertexAttributeData &data):mData(data)
        {
            mOffset=0;
            mIndex=0;
            mStride=0;
        }

        VertexAttribute(const VertexAttributeData& data, ptrdiff_t offset, size_t index=0,size_t stride=0 ):mData(data)
        {
            mOffset=offset;
            mIndex=index;
            mStride=stride;
        }

        inline size_t index() const { return mIndex;}
        inline size_t stride() const { return mStride; }
        inline ptrdiff_t offset() const { return mOffset;}

        const VertexAttributeData& data() const  { return mData; }

        void setOffset(ptrdiff_t mOffset)
        {
            mOffset = mOffset;
        }

        void setIndex(size_t mIndex)
        {
            mIndex = mIndex;
        }

        void setStride(size_t mStride)
        {
            mStride = mStride;
        }



        static const VertexAttributeDataArray Position;
        static const VertexAttributeDataArray PositionColor;
        static const VertexAttributeDataArray PositionNormal;
        static const VertexAttributeDataArray PositionNormalColor;
        static const VertexAttributeDataArray PositionNormalUV1;
        static const VertexAttributeDataArray PositionNormalUV2;
        static const VertexAttributeDataArray PositionUV1;
        static const VertexAttributeDataArray PositionUV2;


    private:

        VertexAttributeData mData;
        ptrdiff_t	mOffset;
        size_t      mIndex;
        size_t      mStride;
    };



#define VertexAttributeArray			rde::vector<VertexAttribute>

	class VertexAttributesList: public RenderResource
	{
	public:	

		



        VertexAttributesList(RenderManager *manager,const VertexAttributeDataArray &data);
        VertexAttributesList(RenderManager *manager,const VertexAttributeData *data, size_t count);


        VertexAttribute* findAttribute(VertexBinding bindingId, size_t index=0) const;

        inline bool hasBinding(VertexBinding  binding)
        {
            return (mBindingMasks[static_cast<size_t>(binding)]!=0);
        }


        inline size_t attributeCount(VertexBinding bindingId)
        {
            return mBindingMasks[static_cast<size_t>(bindingId)];
        }

        inline size_t bindingMask(VertexBinding  binding)
        {
            return mBindingMasks[static_cast<size_t>(binding)];
        }

        inline size_t stride() const { return mStride;}
        inline size_t sizeInBytes() const { return mStride;}



        
        
        static void calculateStrideAndIndex(VertexAttributeArray &array);
        

        

        void updateAfterModification();
    private:
        
        mutable VertexAttributeArray mVertexAttributes;
        size_t mStride;
        size_t mBindingMasks[static_cast<size_t>(VertexBinding::Invalid)];
	};

    using    VertexAttributesListPtr=intrusive_ptr<VertexAttributesList>;


}
#endif
