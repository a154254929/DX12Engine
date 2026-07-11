#include "RenderTarget.h"

FRenderTarget::FRenderTarget()
    : width(256U)
    , height(256U)
    , format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
    ResetViewport(width, height);
    ResetScissorRect(width, height);
}

void FRenderTarget::Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat)
{
    width = inWidth;
    height = inHeight;
    format = inFormat;
    
    ResetViewport(width, height);
    ResetScissorRect(width, height);
    
    BuildRenderTagetMap();
    
    BuildSRVDescriptors();
    BuildRTVDescriptors();
}

void FRenderTarget::ResetRenderTarget(UINT inWidth, UINT inHeight)
{
    if (inWidth != width || inHeight != height)
    {
        width = inWidth;
        height = inHeight;
        
        ResetViewport(width, height);
        ResetScissorRect(width, height);
        
        BuildRenderTagetMap();
        BuildRTVDescriptors();
        BuildSRVDescriptors();
    }
}

void FRenderTarget::ResetViewport(UINT inWidth, UINT inHeight)
{
    viewport = {
        0.0f,
        0.0f,
        (float)width,
        (float)height,
        0.0f,
        1.0f
    };
}

void FRenderTarget::ResetScissorRect(UINT inWidth, UINT inHeight)
{
    scissorRect = {
        0,
        0,
        (LONG)width,
        (LONG)height
    };
}
