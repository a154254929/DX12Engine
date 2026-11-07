#include "RenderBuffer.h"

#include "Engine/Rendering/Core/DirectX/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

FRenderBuffer::FRenderBuffer()
{
    CreateRenderTarget<FBufferRenderTarget>();
}


void FRenderBuffer::Init(
    FGeometryMap* inGeometryMap,
    FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager
)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
    
    //绑定渲染目标的回调函数
    if (FBufferRenderTarget* bufferRenderTarget = dynamic_cast<FBufferRenderTarget*>(renderTarget.get()))
    {
        bufferRenderTarget->renderTargetDelegate.Bind(this, &FRenderBuffer::BuildRenderTargetBuffer);
    }
}

void FRenderBuffer::Init(int inWidth, int inHeight)
{
    Super::Init(inWidth, inHeight);
    
    renderTarget->Init(inWidth, inHeight, format);
}

void FRenderBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& outResource)
{
    
}
