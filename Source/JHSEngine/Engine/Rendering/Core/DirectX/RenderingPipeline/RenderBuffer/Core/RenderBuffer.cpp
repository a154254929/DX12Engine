#include "RenderBuffer.h"

FRenderBuffer::FRenderBuffer()
{
}

void FRenderBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
                         FRenderLayerManager* inRenderLayerManager)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FRenderBuffer::Init(int inWidth, int inHeight)
{
    Super::Init(inWidth, inHeight);
}
