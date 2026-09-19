#pragma once
#include "../Core/DynamicCubeMap.h"

class FDynamicReflectionCubeMap : public FDynamicCubeMap
{
    typedef FDynamicCubeMap Super;
public:
    FDynamicReflectionCubeMap();
    
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
    
    bool IsExistDynamicReflectionMesh();
    
    virtual void BuildDepthStencilDescriptor();
    virtual void BuildRenderTargetDescriptor();
    
protected:
    virtual void BuildRenderTargetRTV();
    virtual void BuildRenderTargetSRV();
};