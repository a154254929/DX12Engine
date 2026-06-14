#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"
#include "../../../../../../Shader/Core/ShaderType.h"

class FGeometryMap;
struct FDirectXPipelineState;
class FViewportInfo;
class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
    friend class FGeometry;
    friend class FGeometryMap;
public:
    FRenderLayer();
    
    void RegisterRenderLayer();
    
    //基础注册环境
    virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState);

    virtual void PreDraw(float deltaTime);
    virtual void Draw(float deltaTime);
    virtual void PostDraw(float deltaTime);
    
    virtual void BuildPSO();

    virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);
public:  
    const UINT GetRenderLayerPriority() const {return renderPriority;}
    
public:
    virtual void BuildShader(){};
    
    virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& intShaderMacro);
    
    virtual int GetRenderLayerType() const = 0;
    

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
