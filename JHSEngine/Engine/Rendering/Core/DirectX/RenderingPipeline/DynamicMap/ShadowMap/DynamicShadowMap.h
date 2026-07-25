#pragma once
#include "../Core/DynamicMap.h"

class GClientViewport;
class FViewportInfo;

class FDynamicShadowMap : public FDynamicMap
{
    typedef FDynamicMap Super;
public:
    FDynamicShadowMap();
    
public:
    virtual void Init(UINT inWidth, UINT inHeight);
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
    );
    
    virtual void UpdateCalculations(
        float deltaTime,
        const FViewportInfo& inViewportInfo
    ); 
    
    virtual void PreDraw(float deltaTime);
    
    virtual void Draw(float deltaTime);
    
public:
    virtual void BuildRenderTargetDescriptor();
    virtual void BuildDepthStencilDescriptor();
    
protected:
    virtual void BuildRenderTargetSRV();
    
};
