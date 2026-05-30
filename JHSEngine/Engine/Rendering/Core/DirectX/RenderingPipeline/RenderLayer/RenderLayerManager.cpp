#include "RenderLayerManager.h"
#include "RenderLayers/AlphaTestRenderLayer.h"
#include "RenderLayers/OpaqueRenderLayer.h"
#include "RenderLayers/PostProcessRenderLayer.h"
#include "RenderLayers/TransparentRenderLayer.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

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

void FRenderLayerManager::PreDraw(float deltaTime)
{
    for (auto& tmp : renderLayers)
    {
        tmp->PreDraw(deltaTime);
    }
}

void FRenderLayerManager::Draw(float deltaTime)
{
    for (auto& tmp : renderLayers)
    {
        tmp->Draw(deltaTime);
    }
}

void FRenderLayerManager::PostDraw(float deltaTime)
{
    for (auto& tmp : renderLayers)
    {
        tmp->PostDraw(deltaTime);
    }
}

void FRenderLayerManager::BuildPSO()
{
    for (auto& tmp : renderLayers)
    {
        tmp->BuildPSO();
    }
}

void FRenderLayerManager::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
    for (auto& tmp : renderLayers)
    {
        tmp->UpdateCalculations(deltaTime, viewportInfo);
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

std::shared_ptr<FRenderLayer> FRenderLayerManager::FindByRenderLayer(int inRenderLayerType)
{
    for (auto& tmp : renderLayers)
    {
        if (tmp->GetRenderLayerType() == inRenderLayerType)
            return tmp;
    }
    return NULL;
}
