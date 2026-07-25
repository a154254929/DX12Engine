#include "DynamicShadowMap.h"

#include "../../RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
    : Super()
{
    CreateRenderTarget<FShadowMapRenderTarget>();
}

void FDynamicShadowMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FDynamicShadowMap::UpdateCalculations(float deltaTime, const FViewportInfo& inViewportInfo)
{

}

void FDynamicShadowMap::PreDraw(float deltaTime)
{

}

void FDynamicShadowMap::Draw(float deltaTime)
{

}
