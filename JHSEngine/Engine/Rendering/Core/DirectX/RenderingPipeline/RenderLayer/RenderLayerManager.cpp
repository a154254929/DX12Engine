#include "RenderLayerManager.h"
#include "RenderLayers/AlphaTestRenderLayer.h"
#include "RenderLayers/OpaqueRenderLayer.h"
#include "RenderLayers/PostProcessRenderLayer.h"
#include "RenderLayers/TransparentRenderLayer.h"
#include "RenderLayers/BackGroundRenderLayer.h"
#include "RenderLayers/OpaqueReflectorRenderLayer.h"
#include "RenderLayers/OpaqueShadowRenderLayer.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

std::vector<shared_ptr<FRenderLayer>> FRenderLayerManager::renderLayers;

FRenderLayerManager::FRenderLayerManager()
{
    renderLayers.clear();
    
    CreateRenderLayer<FAlphaTestRenderLayer>();
    CreateRenderLayer<FOpaqueRenderLayer>();
    CreateRenderLayer<FPostProcessRenderLayer>();
    CreateRenderLayer<FTransparentRenderLayer>();
    CreateRenderLayer<FBackGroundRenderLayer>();
    CreateRenderLayer<FOpaqueReflectorRenderLayer>();
    CreateRenderLayer<FOpaqueShadowRenderLayer>();
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

void FRenderLayerManager::Draw(int inLayer, float deltaTime)
{
    
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->Draw(deltaTime);
    }
}

void FRenderLayerManager::FindObjDraw(int inLayer, float deltaTime, const CMeshComponent* inMeshComponent)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->FindObjDraw(deltaTime, inMeshComponent);
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

void FRenderLayerManager::ResetPSO(int inLayer)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->ResetPSO();
    }
}

void FRenderLayerManager::DrawMesh(float deltaTime, int inLayer, ERenderingConditions inRenderingConditions)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->DrawMesh(deltaTime, inRenderingConditions);
    }
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
