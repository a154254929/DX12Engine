#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"

class FRenderTarget
    : public IDirectXDeviceInterface
    , std::enable_shared_from_this<FRenderTarget>
{
public:
    FRenderTarget();
    
    virtual void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
    
    virtual void ResetRenderTarget(UINT inWidth, UINT inHeight);
    
protected:
    virtual void BuildRenderTargetMap() {};
    virtual void BuildSRVDescriptors() {};
    virtual void BuildRTVDescriptors() {};
    virtual void BuildDSVDescriptors() {};
    
public:
    void ResetViewport(UINT inWidth, UINT inHeight);
    void ResetScissorRect(UINT inWidth, UINT inHeight);
    
    FORCEINLINE ID3D12Resource* GetRenderTaget() const {return renderTargetMap.Get();};
    FORCEINLINE D3D12_VIEWPORT GetViewport() const {return viewport;};
    FORCEINLINE D3D12_RECT GetScissorRect() const {return scissorRect;};

    CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUSRVOffset() {return gpuShaderResourceView;};
    CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUSRVOffset() {return cpuShaderResourceView;};
    
protected:
    UINT width;
    UINT height;
    DXGI_FORMAT format;
    
    D3D12_VIEWPORT viewport;
    D3D12_RECT scissorRect;
    
    ComPtr<ID3D12Resource> renderTargetMap;
    //SRV
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpuShaderResourceView;
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuShaderResourceView;
    
};