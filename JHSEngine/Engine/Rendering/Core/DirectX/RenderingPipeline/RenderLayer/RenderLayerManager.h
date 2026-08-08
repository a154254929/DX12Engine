#pragma once
#include "Core/RenderLayer.h"

class FRenderLayerManager
{
    friend class FRenderLayer;
    friend class FGeometryMap;
    friend class FGeometry;
public:
    FRenderLayerManager();
    
    ~FRenderLayerManager();
    
    virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState);

    virtual void PreDraw(float deltaTime);
    virtual void Draw(float deltaTime);
    virtual void PostDraw(float deltaTime);
    
    //需要单独渲染哪个层
    virtual void Draw(int inLayer, float deltaTime);
    
    virtual void FindObjDraw(int inLayer, float deltaTime, const CMeshComponent* inMeshComponent);

    virtual void BuildPSO();

    virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);
    
    virtual void BuildShader();
    
    void SortRenderLayer();
    
    //单独设置PSO
    virtual void ResetPSO(int inLayer);
    virtual void ResetPSO(int inLayer, EPipelineState inPipelineState);
    //渲染 不包含PSO
    virtual void DrawMesh(float deltaTime, int inLayer, ERenderingConditions inRenderingConditions = ERenderingConditions::RC_None);
    
    static std::shared_ptr<FRenderLayer> FindByRenderLayer(int inRenderLayerType);
protected:
    static std::vector<shared_ptr<FRenderLayer>> renderLayers;
};

template<typename T, typename ... Args>
std::shared_ptr<T> CreateRenderLayer()
{
    std::shared_ptr<T> renderLayer = std::make_shared<T>();
    renderLayer->RegisterRenderLayer();
    
    return renderLayer;
}
