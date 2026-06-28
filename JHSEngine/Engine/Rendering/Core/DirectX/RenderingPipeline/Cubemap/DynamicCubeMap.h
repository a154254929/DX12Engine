#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

class FCubeMapRenderTarget;
class GClientViewport;
class FGeometryMap;
struct FDirectXPipelineState;

class FDynamicCubeMap : public IDirectXDeviceInterface
{
public:
    FDynamicCubeMap();
    
    virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState);
    
protected:
    virtual void BuildViewPort(const fvector_3d& inPosition);
    virtual void BuildDepthStencil();
    
protected:
    std::unique_ptr<FCubeMapRenderTarget> renderTarget;
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDesc;              //dsv描述
    
    vector<GClientViewport*> viewports;                 //6个视口
    
    FGeometryMap* geometryMap;                          //几何Map
    FDirectXPipelineState* directXPipelineState;        //管线对象，用于绑定
    
    ComPtr<ID3D12Resource> depthStencilBuffer;
    
    UINT width;
    UINT height;
};