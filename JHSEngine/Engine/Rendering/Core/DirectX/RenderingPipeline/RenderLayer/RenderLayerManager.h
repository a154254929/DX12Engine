#pragma once
#include "Core/RenderLayer.h"

class FRenderLayerManager
{
public:
    FRenderLayerManager();
    
protected:
    std::vector<shared_ptr<FRenderLayer>> renderLayers;
};
