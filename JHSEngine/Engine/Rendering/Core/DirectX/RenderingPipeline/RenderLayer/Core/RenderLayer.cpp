#include "RenderLayer.h"
#include "../RenderLayerManager.h"

FRenderLayer::FRenderLayer()
    : renderPriority(0)
{
}

void FRenderLayer::RegisterRenderLayer()
{
    FRenderLayerManager::renderLayers.push_back(this->shared_from_this());
}

void FRenderLayer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
    geometryMap = inGeometryMap;
    directXPipelineState = inDirectXPipelineState;
}
