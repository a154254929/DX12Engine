#pragma once
#include "Core/RenderTarget.h"

class FShadowMapRenderTarget : public FRenderTarget
{
    typedef FRenderTarget Super;
public:
    FShadowMapRenderTarget();

    virtual void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
protected:
    virtual void BuildRenderTagetMap();
    virtual void BuildSRVDescriptors();
    virtual void BuildRTVDescriptors();
};