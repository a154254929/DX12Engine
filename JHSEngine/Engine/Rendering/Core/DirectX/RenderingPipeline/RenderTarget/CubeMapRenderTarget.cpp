#include "CubeMapRenderTarget.h"
#include "../../../../../Debug/EngineDebug.h"

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
    CD3DX12_RESOURCE_DESC heapBufferDesc;
    memset(&heapBufferDesc, 0, sizeof(heapBufferDesc));
    heapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    heapBufferDesc.Alignment = 0;
    heapBufferDesc.Width = width;
    heapBufferDesc.Height = height;
    heapBufferDesc.DepthOrArraySize = 6;
    heapBufferDesc.Format = format;
    heapBufferDesc.MipLevels = 1;
    heapBufferDesc.SampleDesc.Count = 1;
    heapBufferDesc.SampleDesc.Quality = 0;
    heapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    heapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    
    CD3DX12_HEAP_PROPERTIES heapBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    
    ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &heapBufferDesc,
        D3D12_RESOURCE_STATE_COMMON,
        NULL,
        IID_PPV_ARGS(renderTargetMap.GetAddressOf())
    ));
}
