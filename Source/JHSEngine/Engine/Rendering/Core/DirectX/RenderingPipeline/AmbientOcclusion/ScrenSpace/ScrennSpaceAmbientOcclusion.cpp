#include "ScrennSpaceAmbientOcclusion.h"

void FScreenSpaceAmbientOcclusion::Init(
    FGeometryMap* inGeometryMap,
    FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager
)
{
    normalBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FScreenSpaceAmbientOcclusion::Init(int inWidth, int inHeight)
{
    normalBuffer.Init(inWidth, inHeight);
}

void FScreenSpaceAmbientOcclusion::Draw(float deltaTime)
{
    normalBuffer.Draw(deltaTime);
}

void FScreenSpaceAmbientOcclusion::UpdateCalculations(float deltaTime, const FViewportInfo& inViewportInfo)
{
    normalBuffer.UpdateCalculations(deltaTime, inViewportInfo);
}

void FScreenSpaceAmbientOcclusion::BuildDescriptors()
{
    normalBuffer.BuildDescriptors();
    normalBuffer.BuildRenderTargetRTV();
    normalBuffer.BuildSRVDescriptors();
    normalBuffer.BuildRTVDescriptors();
}
