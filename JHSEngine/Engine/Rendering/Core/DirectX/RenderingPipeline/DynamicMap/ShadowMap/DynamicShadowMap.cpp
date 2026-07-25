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

void FDynamicShadowMap::BuildDepthStencilDescriptor()
{
    UINT dsvDescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(renderTarget.get()))
    {
        inRenderTarget->dsvDescHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
            1                               //主视口DSV
            + 1,                            //Cubemap DSV
            dsvDescriptorHandleIncrementSize
        );
    }
}
