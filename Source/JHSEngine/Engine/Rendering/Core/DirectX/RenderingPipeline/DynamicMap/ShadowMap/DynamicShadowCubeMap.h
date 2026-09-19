#pragma once
#include "../Core/DynamicCubeMap.h"

class GClientViewport;

class FDynamicShadowCubeMap : public FDynamicCubeMap
{
    typedef FDynamicCubeMap Super;
public:
    FDynamicShadowCubeMap();
    
    virtual void UpdateCalculations(
        float deltaTime,
        const FViewportInfo& inViewportInfo
        ); 
    
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
    );
    
    virtual void PreDraw(float deltaTime);
    
    virtual void Draw(float deltaTime);
    
public:
    
    virtual void BuildDepthStencilDescriptor();
    
protected:
    virtual void BuildRenderTargetRTV();
    virtual void BuildRenderTargetSRV();
};
