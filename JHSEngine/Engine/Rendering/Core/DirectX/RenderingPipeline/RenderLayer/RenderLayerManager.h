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

    void PreDraw(float deltaTime);
    void Draw(float deltaTime);
    void PostDraw(float deltaTime);

    virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);
    
    virtual void BuildShader();
    
    void SortRenderLayer();
    
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
