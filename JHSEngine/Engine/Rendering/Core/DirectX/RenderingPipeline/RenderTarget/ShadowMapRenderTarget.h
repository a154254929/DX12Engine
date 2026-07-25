#pragma once
#include "Core/RenderTarget.h"

class FShadowMapRenderTarget : public FRenderTarget
{
    typedef FRenderTarget Super;
    
    friend class FDynamicShadowMap;
public:
    FShadowMapRenderTarget();

    virtual void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
protected:
    virtual void BuildRenderTagetMap();
    virtual void BuildSRVDescriptors();
    virtual void BuildDSVDescriptors();
    
protected:
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDescHandle;              //dsv描述
};