#include "RenderLayerManager.h"
#include "RenderLayers/AlphaTestRenderLayer.h"
#include "RenderLayers/OpaqueRenderLayer.h"
#include "RenderLayers/PostProcessRenderLayer.h"
#include "RenderLayers/TransparentRenderLayer.h"

std::vector<shared_ptr<FRenderLayer>> FRenderLayerManager::renderLayers;

FRenderLayerManager::FRenderLayerManager()
{
    renderLayers.clear();
    
    CreateRenderLayer<FAlphaTestRenderLayer>();
    CreateRenderLayer<FOpaqueRenderLayer>();
    CreateRenderLayer<FPostProcessRenderLayer>();
    CreateRenderLayer<FTransparentRenderLayer>();
}

FRenderLayerManager::~FRenderLayerManager()
{
    renderLayers.clear();
}

void FRenderLayerManager::SortRenderLayer()
{
}
