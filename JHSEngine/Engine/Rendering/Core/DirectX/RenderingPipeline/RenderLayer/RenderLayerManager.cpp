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

void FRenderLayerManager::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
    for (auto& tmp : renderLayers)
    {
        tmp->Init(inGeometryMap, inDirectXPipelineState);
    }
}

void FRenderLayerManager::BuildShader()
{
    for (auto& tmp : renderLayers)
    {
        tmp->BuildShader();
    }
}

void FRenderLayerManager::SortRenderLayer()
{
    auto compRenderLayer = [&](const std::shared_ptr<FRenderLayer> renderLayerA, std::shared_ptr<FRenderLayer> renderLayerB)
    {
        return renderLayerA->GetRenderLayerPriority() < renderLayerB->GetRenderLayerPriority();
    };
    std::sort(renderLayers.begin(), renderLayers.end(), compRenderLayer);
}
