#pragma once
#include "DynamicMap.h"

class GClientViewport;

class FDynamicCubeMap : public FDynamicMap
{
typedef FDynamicMap Super;
protected:
    struct FTmpViewportCapture
    {
        FTmpViewportCapture();
        FTmpViewportCapture(const fvector_3d& inPosition);
        
        fvector_3d targetPosition[6];
        fvector_3d up[6] = {
            fvector_3d(0, 1, 0),
            fvector_3d(0, 1, 0),
            fvector_3d(0, 0, -1),
            fvector_3d(0, 0, 1),
            fvector_3d(0, 1, 0),
            fvector_3d(0, 1, 0),
        };
        
        void BuildViewPortCapture(const fvector_3d& inPosition);
    };
public:
    FDynamicCubeMap();
    
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
    );
    
public:
    virtual void BuildViewPort(const fvector_3d& inPosition);
    virtual void BuildDepthStencil();
    
    void SetViewportPosition(const fvector_3d& inPosition);
    
    virtual void BuildRenderTargetDescriptor();
    
protected:
    virtual void BuildRenderTargetRTV(){};
    virtual void BuildRenderTargetSRV(){};
    
protected:
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDesc;              //dsv描述
    
    vector<GClientViewport*> viewports;                 //6个视口
    
    ComPtr<ID3D12Resource> depthStencilBuffer;
    
};