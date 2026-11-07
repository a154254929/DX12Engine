#pragma once
#include "Core/RenderBuffer.h"

class FNormalBuffer : public FRenderBuffer
{
    typedef FRenderBuffer Super;
public:
    FNormalBuffer();
    
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
    ) override;
    
    virtual void Init(int inWidth, int inHeight) override;
    
    void Draw(float deltaTime);
    
    virtual void BuildDescriptors();
    virtual void BuildRenderTargetRTV();
    virtual void BuildSRVDescriptors();
    virtual void BuildRTVDescriptors();
    
protected:
    virtual void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& outResource);
};