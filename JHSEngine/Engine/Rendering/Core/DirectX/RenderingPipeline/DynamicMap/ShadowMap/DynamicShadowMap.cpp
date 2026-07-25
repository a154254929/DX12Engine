#include "DynamicShadowMap.h"

#include "../../RenderTarget/ShadowMapRenderTarget.h"
#include "../../Geometry/GeometryMap.h"
#include "../../RenderLayer/RenderLayerManager.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Component/Light/Core/LightComponent.h"
#include "../../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../../Component/Light/ParallelLightComponent.h"
#include "../../../../../../Component/Light/PointLightComponent.h"
#include "../../../../../../Manager/LightManager.h"

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
    Super::PreDraw(deltaTime);
    
    if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(renderTarget.get()))
    {
        CD3DX12_RESOURCE_BARRIER resourceBarrierPresent = 
            CD3DX12_RESOURCE_BARRIER::Transition(
                renderTarget->GetRenderTaget(),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                D3D12_RESOURCE_STATE_DEPTH_WRITE
            );
        
        GetGraphicsCommandList()->ResourceBarrier(1, &resourceBarrierPresent);
        
        //需要每帧执行,绑定矩形框
        D3D12_VIEWPORT rtViewPort = renderTarget->GetViewport();
        D3D12_RECT rtScissorRectt = renderTarget->GetScissorRect();
        GetGraphicsCommandList()->RSSetViewports(1, &rtViewPort);
        GetGraphicsCommandList()->RSSetScissorRects(1, &rtScissorRectt);
    
        UINT cbvSize = geometryMap->viewportConstantBufferView.GetConstantBufferByteSize();
        
        //清除深度模板缓冲区
        GetGraphicsCommandList()->ClearDepthStencilView(
            inRenderTarget->dsvDescHandle,
            D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
            1.0f , 0, 0, NULL
        );
        
        //输出的合并阶段
        GetGraphicsCommandList()->OMSetRenderTargets(0,
            nullptr,
            false,
            &inRenderTarget->dsvDescHandle
        );
        
        //更新/绑定摄像机
        D3D12_GPU_VIRTUAL_ADDRESS gpuViewportAddress = geometryMap->viewportConstantBufferView.GetBuffer()->GetGPUVirtualAddress();
        gpuViewportAddress += (
            1
            + geometryMap->GetDynamicReflectionMeshObjectNumber()
        ) * cbvSize;
        GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, gpuViewportAddress);
        
        DrawShadowMapTexture(deltaTime);
        
        renderLayerManager->ResetPSO(RENDERLAYER_OPAQUE_SHADOW);
        
        renderLayerManager->DrawMesh(RENDERLAYER_OPAQUE, deltaTime);
        
        CD3DX12_RESOURCE_BARRIER resourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
            renderTarget->GetRenderTaget(),
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            D3D12_RESOURCE_STATE_GENERIC_READ
        );
        GetGraphicsCommandList()->ResourceBarrier(1, &resourceBarrierPresentRenderTarget);
    }
}

void FDynamicShadowMap::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void FDynamicShadowMap::GetViewportViewMatrix(XMFLOAT4X4& outViewMatrix, XMFLOAT4X4& outProjMatrix)
{
    if (viewport)
    {
        outViewMatrix = viewport->viewMatrix;
        outProjMatrix = viewport->projectMatrix;
    }
}

void FDynamicShadowMap::DrawShadowMapTexture(float deltaTime)
{
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
        7,
        renderTarget->GetGPUOffset()
    );
    
}

void FDynamicShadowMap::SetViewportPosition(const fvector_3d& inPosition)
{
    viewport->SetPosition(XMFLOAT3(inPosition.x, inPosition.y, inPosition.z));
    BuildViewMaterix(0.3f);
}

void FDynamicShadowMap::SetViewportROtation(const fvector_3d& inRotation)
{
    viewport->SetRotation(inRotation);
    BuildViewMaterix(0.3f);
}

void FDynamicShadowMap::BuildViewMaterix(float deltaTime)
{
    viewport->BuildViewMatrix(deltaTime);
}

void FDynamicShadowMap::BuildParallelLightMaterix(
    const fvector_3d& inRotation,
    const fvector_3d& inTargetPosition,
    float inRadius)
{
}

void FDynamicShadowMap::BuildViewPort(const fvector_3d& inPosition)
{
    viewport = CreateObject<GClientViewport>(new GClientViewport());
    viewport->SetPosition(XMFLOAT3(inPosition.x, inPosition.y, inPosition.z));
    viewport->LookAt(inPosition, fvector_3d(10.f), fvector_3d(0.f, 1.f, 0.f));
    viewport->SetFrustum(.05 * XM_PI, 1.0f, 0.1f, 10000.f);
    BuildViewMaterix(0.3f);
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
        //主要是创建Shadow常量缓冲区
        D3D12_CPU_DESCRIPTOR_HANDLE cpuRTVDesAddr = geometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuSRVDesAddr = geometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
        
        //后期要更新
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
