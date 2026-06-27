#include "CubeMapRenderTarget.h"

FCubeMapRenderTarget::FCubeMapRenderTarget()
    : width(256U)
    , height(256U)
    , format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
    ResetViewport(width, height);
}

void FCubeMapRenderTarget::Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat)
{
    width = inWidth;
    height = inHeight;
    format = inFormat;
    
    ResetViewport(width, height);
}

void FCubeMapRenderTarget::ResetViewport(UINT inWidth, UINT inHeight)
{
    viewport = {
        0.0f,
        0.0f,
        (float)width,
        (float)height,
        0.0f,
        1.0f
    };
    
    scissorRect = {
        0,
        0,
        (LONG)width,
        (LONG)height
    };
}

void FCubeMapRenderTarget::BuildRenderTagetMap()
{
    
}
