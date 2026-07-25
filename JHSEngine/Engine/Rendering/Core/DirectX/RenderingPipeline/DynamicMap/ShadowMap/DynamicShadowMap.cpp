#include "DynamicShadowMap.h"

#include "../../RenderTarget/ShadowMapRenderTarget.h"
#include "../../Geometry/GeometryMap.h"
#include "../../RenderLayer/RenderLayerManager.h"
#include "../../../../../../Config/EngineRenderConfig.h"

FDynamicShadowMap::FDynamicShadowMap()
    : Super()
{
    CreateRenderTarget<FShadowMapRenderTarget>();
}

void FDynamicShadowMap::Init(UINT inWidth, UINT inHeight)
{
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

void FDynamicShadowMap::BuildRenderTargetDescriptor()
{
    BuildRenderTargetSRV();
    renderTarget->Init(width, height, DXGI_FORMAT_D24_UNORM_S8_UINT);
}

void FDynamicShadowMap::BuildDepthStencilDescriptor()
{
    if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(renderTarget.get()))
    {
        UINT dsvDescSize = GetDescriptorHandleIncrementSizeByDSV();
        inRenderTarget->dsvDescHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
            1                               //主视口DSV
            + 1,                            //Cubemap DSV
            dsvDescSize
        );
    }
}

void FDynamicShadowMap::BuildRenderTargetSRV()
{
    if (FShadowMapRenderTarget* shadowMapRT = dynamic_cast<FShadowMapRenderTarget*>(renderTarget.get()))
    {
        D3D12_CPU_DESCRIPTOR_HANDLE cpuRTVDesAddr = geometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuSRVDesAddr = geometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
        
        UINT dsvDescSize = GetDescriptorHandleIncrementSizeByDSV();
        shadowMapRT->cpuShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            cpuRTVDesAddr,
            geometryMap->GetDrawTexture2DResourcesNumber()
                + geometryMap->GetDrawTextureCubemapResourcesNumber()
                + 1, //Cubemap 
            dsvDescSize
        );
    
        shadowMapRT->gpuShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
            gpuSRVDesAddr,
            geometryMap->GetDrawTexture2DResourcesNumber()
                + geometryMap->GetDrawTextureCubemapResourcesNumber()
                + 1, //Cubemap
            dsvDescSize
        );
    }
}
