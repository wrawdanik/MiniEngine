
#include "GenericRenderables.h"

using namespace MiniEngine;


RenderablePlane::RenderablePlane(RenderManager *manager,const VertexAttributeDataArray &vertexDefinition):Renderable(manager)
{
    create(vertexDefinition);
}

void RenderablePlane::create(const VertexAttributeDataArray &vertexDefinition)
{
    VertexBufferDescriptor vDescriptor(vertexDefinition,BufferItemRange(0,6),RenderPrimitive::Triangle);

    setValid(createRenderable(vDescriptor).get()== nullptr);

    if(isValid())
    {

    }

}