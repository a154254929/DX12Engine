#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

class FCubeMapRenderTarget
    : public IDirectXDeviceInterface
    , std::enable_shared_from_this<FCubeMapRenderTarget>
{
public:
    FCubeMapRenderTarget();
    
    void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
    
    void ResetViewport(UINT inWidth, UINT inHeight);
    
    void BuildRenderTagetMap();
    
public:
    FORCEINLINE ID3D12Resource* GetRenderTagetMap() const {return renderTargetMap.Get();};
    FORCEINLINE D3D12_VIEWPORT GetViewport() const {return viewport;};
    FORCEINLINE D3D12_RECT GetScissorRect() const {return scissorRect;};
    
private:
    UINT width;
    UINT height;
    DXGI_FORMAT format;
    
    D3D12_VIEWPORT viewport;
    D3D12_RECT scissorRect;
    
    ComPtr<ID3D12Resource> renderTargetMap;
};