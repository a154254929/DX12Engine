#include "ShadowMapRenderTarget.h"

FShadowMapRenderTarget::FShadowMapRenderTarget()
    : Super()
{
}

void FShadowMapRenderTarget::Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat)
{
    Super::Init(inWidth, inHeight, inFormat);
}

void FShadowMapRenderTarget::BuildRenderTagetMap()
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
    heapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    
    CD3DX12_HEAP_PROPERTIES heapBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    
    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;
    clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    
    GetD3dDevice()->CreateCommittedResource(
        &heapBufferProperties,
        D3D12_HEAP_FLAG_NONE,
        &heapBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        &clearValue,
        IID_PPV_ARGS(renderTargetMap.GetAddressOf())
    );
    
}

void FShadowMapRenderTarget::BuildSRVDescriptors()
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.f;
    srvDesc.Texture2D.PlaneSlice = 0.f;
    
    GetD3dDevice()->CreateShaderResourceView(
        renderTargetMap.Get(),
        &srvDesc,
        cpuShaderResourceView
    );
}

void FShadowMapRenderTarget::BuildDSVDescriptors()
{
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    GetD3dDevice()->CreateDepthStencilView(
        renderTargetMap.Get(),
        &dsvDesc,
        dsvDescHandle
    );
}
