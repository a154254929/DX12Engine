#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"

class FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
    friend class FGeometry;
    friend class FGeometryMap;
public:
    FRenderLayer();
    
    void RegisterRenderLayer();
    
    //基础注册环境
    virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState);
public:  
    const UINT GetRenderLayerPriority() const {return renderPriority;}
    
public:
    virtual void BuildShader() = 0;
    
    virtual int FetRenderLayerType() = 0;
    
protected:
    UINT renderPriority;
protected:
    FShader vertexShader;
    FShader pixelShader;
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDesc;
    
    std::vector<FRenderingData> renderingDatas;
    
    FGeometryMap* geometryMap;
    FDirectXPipelineState* directXPipelineState;
};
