#include "CubeMapRenderTarget.h"
#include "../../../../../Debug/EngineDebug.h"

FCubeMapRenderTarget::FCubeMapRenderTarget()
    : width(256U)
    , height(256U)
    , format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
    ResetViewport(width, height);
    ResetScissorRect(width, height);
    
    cpuRenderTargetView.resize(6);
}

void FCubeMapRenderTarget::Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat)
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
}

void FCubeMapRenderTarget::ResetScissorRect(UINT inWidth, UINT inHeight)
{
    scissorRect = {
        0,
        0,
        (LONG)width,
        (LONG)height
    };
}

void FCubeMapRenderTarget::ResetRenderTarget(UINT inWidth, UINT inHeight)
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
    
    GetD3dDevice()->CreateCommittedResource(
        &heapBufferProperties,
        D3D12_HEAP_FLAG_NONE,
        &heapBufferDesc,
        D3D12_RESOURCE_STATE_COMMON,
        NULL,
        IID_PPV_ARGS(renderTargetMap.GetAddressOf())
    );
}

void FCubeMapRenderTarget::BuildSRVDescriptors()
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.MipLevels = 1;
    srvDesc.TextureCube.ResourceMinLODClamp = 0;
    
    GetD3dDevice()->CreateShaderResourceView(
        renderTargetMap.Get(),
        &srvDesc,
        cpuShaderResourceView
    );
}

void FCubeMapRenderTarget::BuildRTVDescriptors()
{
    for (size_t i = 0; i < 6; ++i)
    {
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = format;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
    
        rtvDesc.Texture2DArray.MipSlice = 0;
        rtvDesc.Texture2DArray.PlaneSlice = 0;
        rtvDesc.Texture2DArray.FirstArraySlice = i;
        rtvDesc.Texture2DArray.ArraySize = 1;
        
    
        GetD3dDevice()->CreateRenderTargetView(
            renderTargetMap.Get(),
            &rtvDesc,
            cpuRenderTargetView[i]
        );
    }
}
