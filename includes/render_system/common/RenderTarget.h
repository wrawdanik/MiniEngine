//
// Created by Walter Rawdanik on 10/21/15.
//

#ifndef MINIENGINE_RENDERTARGET_H
#define MINIENGINE_RENDERTARGET_H

#include "RenderResource.h"
#include "Viewport.h"


namespace MiniEngine
{
    class RenderManager;
    class Viewport;

    class RenderTarget: public Resource
    {

    public:

        RenderTarget(RenderManager *manager):Resource(manager) {}
        virtual ~RenderTarget()=default;

        virtual void render()=0;

        inline PixelFormat pixelFormat() const  { return mPixelFormat;}
        inline Viewport viewport() const  { return mViewport;}

    protected:

        void setViewport(const Viewport &view)
        {
            mViewport=view;
        }

        void setPixelFormat(PixelFormat pf)
        {
            mPixelFormat=pf;
        }

    private:
        Viewport mViewport;
        PixelFormat mPixelFormat;
    };

    using    RenderTargetPtr=intrusive_ptr<RenderTarget>;
}

#endif //MINIENGINE_RENDERTARGET_H
