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

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& outResource)
{
    CD3DX12_RESOURCE_DESC heapBufferDesc;
    memset(&heapBufferDesc, 0, sizeof(heapBufferDesc));
    heapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    heapBufferDesc.Alignment = 0;
    heapBufferDesc.Width = width;
    heapBufferDesc.Height = height;
    heapBufferDesc.DepthOrArraySize = 1;
    heapBufferDesc.Format = format;
    heapBufferDesc.MipLevels = 1;
    heapBufferDesc.SampleDesc.Count = 1;
    heapBufferDesc.SampleDesc.Quality = 0;
    heapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    heapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    
    CD3DX12_HEAP_PROPERTIES heapBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    
    const float defaultClearColor[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
    CD3DX12_CLEAR_VALUE clearValue(format, defaultClearColor);
    
    GetD3dDevice()->CreateCommittedResource(
        &heapBufferProperties,
        D3D12_HEAP_FLAG_NONE,
        &heapBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        &clearValue,
        IID_PPV_ARGS(outResource.GetAddressOf())
    );
}
