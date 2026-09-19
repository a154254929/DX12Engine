#include "DynamicMap.h"

#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../RenderLayer/RenderLayerManager.h"

FDynamicMap::FDynamicMap()
    : geometryMap(NULL)
    , directXPipelineState(NULL)
    , renderLayerManager(NULL)
    , width(256U)
    , height(256U)
{
}

void FDynamicMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager)
{
    geometryMap = inGeometryMap;
    directXPipelineState = inDirectXPipelineState;
    renderLayerManager = inRenderLayerManager;
}

void FDynamicMap::UpdateCalculations(float deltaTime, const FViewportInfo& inViewportInfo)
{
}

void FDynamicMap::PreDraw(float deltaTime)
{
}

void FDynamicMap::Draw(float deltaTime)
{
}
