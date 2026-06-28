#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

class FCubeMapRenderTarget
    : public IDirectXDeviceInterface
    , std::enable_shared_from_this<FCubeMapRenderTarget>
{
    friend class FDynamicCubeMap;
public:
    FCubeMapRenderTarget();
    
    void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
    
    void ResetViewport(UINT inWidth, UINT inHeight);
    void ResetScissorRect(UINT inWidth, UINT inHeight);
    
    void ResetRenderTarget(UINT inWidth, UINT inHeight);
    
private:
    void BuildRenderTagetMap();
    void BuildSRVDescriptors();
    void BuildRTVDescriptors();
    
public:
    FORCEINLINE ID3D12Resource* GetRenderTaget() const {return renderTargetMap.Get();};
    FORCEINLINE D3D12_VIEWPORT GetViewport() const {return viewport;};
    FORCEINLINE D3D12_RECT GetScissorRect() const {return scissorRect;};
    
private:
    UINT width;
    UINT height;
    DXGI_FORMAT format;
    
    D3D12_VIEWPORT viewport;
    D3D12_RECT scissorRect;
    
    ComPtr<ID3D12Resource> renderTargetMap;
    
    //SRV
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpuShaderResourceView;
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuShaderResourceView;
    
    //RTV
    std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> cpuRenderTargetView;
};