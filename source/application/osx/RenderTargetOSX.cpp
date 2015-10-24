//
// Created by Walter Rawdanik on 10/21/15.
//
#include "RenderTargetOSX.h"

using namespace MiniEngine;

RenderTargetOSX::RenderTargetOSX(RenderManager *manager,const Viewport &viewport, const PixelFormat &pixelFormat)
        :RenderTarget(manager)
{
    setViewport(viewport);
    setPixelFormat(pixelFormat);
}
RenderTargetOSX::~RenderTargetOSX()
{

}

void RenderTargetOSX::render()
{

}
