

#include "VertexAttributes.h"
#include "MiscUtils.h"



using namespace MiniEngine;

const VertexAttributeDataArray VertexAttribute::Position{VertexAttributeData(VertexBinding::Position,3,RenderData::Float)};
const VertexAttributeDataArray VertexAttribute::PositionColor{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                              VertexAttributeData(VertexBinding::Color,4,RenderData::UByte,RenderBufferMode::Static,true)};

const VertexAttributeDataArray VertexAttribute::PositionNormal{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                              VertexAttributeData(VertexBinding::Normal,3,RenderData::Float,RenderBufferMode::Static,false)};

const VertexAttributeDataArray VertexAttribute::PositionNormalColor{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                                     VertexAttributeData(VertexBinding::Normal,3,RenderData::Float,RenderBufferMode::Static,false),
                                                                     VertexAttributeData(VertexBinding::Normal,3,RenderData::Float,RenderBufferMode::Static,false)};

const VertexAttributeDataArray VertexAttribute::PositionNormalUV1{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                                    VertexAttributeData(VertexBinding::Normal,3,RenderData::Float,RenderBufferMode::Static,false),
                                                                    VertexAttributeData(VertexBinding::UV,2,RenderData::Float,RenderBufferMode::Static,false)};

const VertexAttributeDataArray VertexAttribute::PositionNormalUV2{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                                  VertexAttributeData(VertexBinding::Normal,3,RenderData::Float,RenderBufferMode::Static,false),
                                                                  VertexAttributeData(VertexBinding::UV,2,RenderData::Float,RenderBufferMode::Static,false),
                                                                  VertexAttributeData(VertexBinding::UV,2,RenderData::Float,RenderBufferMode::Static,false)};

const VertexAttributeDataArray VertexAttribute::PositionUV1{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                                  VertexAttributeData(VertexBinding::UV,2,RenderData::Float,RenderBufferMode::Static,false)};

const VertexAttributeDataArray VertexAttribute::PositionUV2{VertexAttributeData(VertexBinding::Position,3,RenderData::Float),
                                                            VertexAttributeData(VertexBinding::UV,2,RenderData::Float,RenderBufferMode::Static,false),
                                                            VertexAttributeData(VertexBinding::UV,2,RenderData::Float,RenderBufferMode::Static,false)};


VertexAttributesList::VertexAttributesList(RenderManager* manager,const VertexAttributeDataArray &data):VertexAttributesList(manager,&data[0],data.size())
{
}
VertexAttributesList::VertexAttributesList(RenderManager* manager,const VertexAttributeData *data, size_t count):mVertexAttributes(count),
																												 RenderResource(manager)
{

	for(size_t index=0;index<count;index++)
	{
		mVertexAttributes[index]=VertexAttribute(*data++);
		index++;
	}
	calculateStrideAndIndex(mVertexAttributes);
}






void VertexAttributesList::updateAfterModification()
{

    size_t indices[static_cast<size_t>(VertexBinding::Invalid)];

    for(size_t i=0;i<static_cast<size_t>(VertexBinding::Invalid);i++)
    {
		mBindingMasks[i]=0;
        indices[i]=0;
    }    
    
    VertexAttributeArray::iterator curr=mVertexAttributes.begin();
    VertexAttributeArray::iterator last=mVertexAttributes.end();
    
    while(curr!=last)
    {
		const size_t bindingOffset=static_cast<size_t>(curr->data().binding());

		mBindingMasks[bindingOffset]|=(indices[bindingOffset]<<bindingOffset);
        indices[bindingOffset]++;
        ++curr;
    }
}


VertexAttribute* VertexAttributesList::findAttribute(VertexBinding bindingId,size_t index) const
{
	VertexAttributeArray::iterator itCurrent=mVertexAttributes.begin();
	VertexAttributeArray::iterator itEnd=mVertexAttributes.end();

	while(itCurrent!=itEnd)
	{
		if((*itCurrent).data().binding()==bindingId && (*itCurrent).index()==index)
		{
				return &(*itCurrent);
		}		
		itCurrent++;
	}
	return nullptr;
}



void VertexAttributesList::calculateStrideAndIndex(VertexAttributeArray &array)
{
    if(array.empty() )
        return;
    
    size_t stride=0;
    size_t indexCount[static_cast<size_t>(VertexBinding::Invalid)];
    memset(indexCount,0,static_cast<size_t>(VertexBinding::Invalid)*sizeof(size_t));

	for(auto &vertexAtt:array)
		stride+=vertexAtt.data().sizeInBytes();

	ptrdiff_t offset=0;
	for(auto &vertexAtt:array)
    {
		const size_t bindingOffset=static_cast<size_t>(vertexAtt.data().binding());
		offset+=vertexAtt.data().sizeInBytes();
		vertexAtt.setStride(stride);
		vertexAtt.setIndex(indexCount[bindingOffset]);
		vertexAtt.setOffset(offset);
        indexCount[bindingOffset]++;;
    }
}




