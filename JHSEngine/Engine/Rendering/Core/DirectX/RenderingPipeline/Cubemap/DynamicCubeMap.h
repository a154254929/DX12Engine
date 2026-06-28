#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../RenderTarget/CubeMapRenderTarget.h"

class GClientViewport;
class FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManager;
class FViewportInfo;

class FDynamicCubeMap : public IDirectXDeviceInterface
{
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
    
    virtual void UpdateCalculations(
        float deltaTime,
        const FViewportInfo& inViewportInfo
    ); 
    
    virtual void PreDraw(float deltaTime);
    
    virtual void Draw(float deltaTime);
    
public:
    void SetCubemapViewportPosition(const fvector_3d& inPosition);
    
    virtual void BuildViewPort(const fvector_3d& inPosition);
    virtual void BuildDepthStencil();
    
    virtual void BuildDepthStencilDescriptor();
    virtual void BuildRenderTargetDescriptor();
    
protected:
    virtual void BuildRenderTargetRTV();
    virtual void BuildRenderTargetSRV();
    
protected:
    std::unique_ptr<FCubeMapRenderTarget> renderTarget;
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDesc;              //dsv描述
    
    vector<GClientViewport*> viewports;                 //6个视口
    
    FGeometryMap* geometryMap;                          //几何Map
    FDirectXPipelineState* directXPipelineState;        //管线对象，用于绑定
    
    FRenderLayerManager* renderLayerManager;                          //渲染层级
    
    ComPtr<ID3D12Resource> depthStencilBuffer;
    
    UINT width;
    UINT height;
};