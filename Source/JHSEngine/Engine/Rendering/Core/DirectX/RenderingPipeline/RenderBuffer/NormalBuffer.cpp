#include "NormalBuffer.h"

FNormalBuffer::FNormalBuffer()
{
    format = DXGI_FORMAT_R16G16B16A16_FLOAT;
}

void FNormalBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FNormalBuffer::Init(int inWidth, int inHeight)
{
    Super::Init(inWidth, inHeight);
}

void FNormalBuffer::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void FNormalBuffer::BuildDescriptors()
{
}

void FNormalBuffer::BuildRenderTargetRTV()
{
}

void FNormalBuffer::BuildSRVDescriptors()
{
}

void FNormalBuffer::BuildRTVDescriptors()
{
}

void FNormalBuffer::BuildRenderTargetBuffrer(ComPtr<ID3D12Resource>& outResource)
{
}
