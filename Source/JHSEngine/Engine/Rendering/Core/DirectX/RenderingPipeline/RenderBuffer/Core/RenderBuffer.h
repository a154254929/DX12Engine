#pragma once
#include "../../DynamicMap/Core/DynamicMap.h"
#include "../../RenderTarget/Core/RenderTarget.h"

class FRenderBuffer : public FDynamicMap
{
    typedef FDynamicMap Super;
public:
    FRenderBuffer();
    
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
    ) override;
    
    virtual void Init(int inWidth, int inHeight) override;
    
protected:
    DXGI_FORMAT format;
    
};