//
// Created by Walter Rawdanik on 10/21/15.
//

#include "RenderTarget.h"


namespace MiniEngine
{
    class RenderManager;
    class Viewport;

    class RenderTargetOSX: public RenderTarget
    {

    public:

        RenderTargetOSX(RenderManager *manager,const Viewport &viewport, const PixelFormat &pixelFormat=PixelFormat::BGRA_8888());
        virtual ~RenderTargetOSX();

        virtual void render();

    };
}



