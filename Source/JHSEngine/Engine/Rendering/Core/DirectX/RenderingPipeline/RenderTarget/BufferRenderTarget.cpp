#include "BufferRenderTarget.h"

void FBufferRenderTarget::Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat)
{
    Super::Init(inWidth, inHeight, inFormat);
}

FBufferRenderTarget::FBufferRenderTarget()
{
    
}

void FBufferRenderTarget::BuildRenderTargetMap()
{
    if (renderTargetDelegate.IsBound())
    {
        renderTargetDelegate.Execute(renderTargetMap);
    }
}

void FBufferRenderTarget::BuildSRVDescriptors()
{
    Super::BuildSRVDescriptors();
}

void FBufferRenderTarget::BuildRTVDescriptors()
{
    Super::BuildRTVDescriptors();
}
