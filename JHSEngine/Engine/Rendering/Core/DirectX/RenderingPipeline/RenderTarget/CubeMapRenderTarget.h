#pragma once
#include "Core/RenderTarget.h"

class FCubeMapRenderTarget : public FRenderTarget
{
    typedef FRenderTarget Super;
    friend class FDynamicCubeMap;
public:
    FCubeMapRenderTarget();
    
    virtual void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
    
    virtual void ResetRenderTarget(UINT inWidth, UINT inHeight);
    
private:
    virtual void BuildRenderTagetMap();
    virtual void BuildSRVDescriptors();
    virtual void BuildRTVDescriptors();
    
public:
    
private:
    //RTV
    std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> cpuRenderTargetView;
    
};