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
    
    void GetViewportViewMatrix(XMFLOAT4X4& outViewMatrix, XMFLOAT4X4& outProjMatrix);
    
    virtual void DrawShadowMapTexture(float deltaTime);
public:
    void SetViewportPosition(const fvector_3d& inPosition);
    void SetViewportROtation(const fvector_3d& inRotation);
    void BuildViewMaterix(float deltaTime);
    void BuildParallelLightMaterix(
        const fvector_3d& inDirection,
        const fvector_3d& inTargetPosition,
        float inRadius = 100.f
    );
    
    virtual void BuildViewPort(const fvector_3d& inPosition);
    
public:
    virtual void BuildRenderTargetDescriptor();
    virtual void BuildDepthStencilDescriptor();
    
protected:
    virtual void BuildRenderTargetSRV();
    
protected:
    GClientViewport* viewport;
    
};
