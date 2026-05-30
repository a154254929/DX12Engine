#pragma once
#include "Core/RenderLayer.h"

class FRenderLayerManager
{
    friend class FRenderLayer;
public:
    FRenderLayerManager();
    
    ~FRenderLayerManager();
    
    void SortRenderLayer();
    
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
