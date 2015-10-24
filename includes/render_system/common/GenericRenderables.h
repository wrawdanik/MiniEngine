//
// Created by Walter Rawdanik on 10/23/15.
//

#ifndef MINIENGINE_GENERICRENDERABLES_H
#define MINIENGINE_GENERICRENDERABLES_H

#include "Renderable.h"

namespace MiniEngine
{


    class RenderablePlane:public Renderable
    {
    public:
        RenderablePlane(RenderManager *manager,const VertexAttributeDataArray &vertexDefinition=VertexAttribute::PositionNormal);

    private:

        void create(const VertexAttributeDataArray &vertexDefinition);

    };
}

#endif //MINIENGINE_GENERICRENDERABLES_H
