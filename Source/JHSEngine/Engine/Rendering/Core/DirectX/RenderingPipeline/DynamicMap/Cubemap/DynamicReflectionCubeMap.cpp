#include "DynamicReflectionCubeMap.h"

#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../Geometry/GeometryMap.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"
#include "../../RenderLayer/RenderLayerManager.h"

FDynamicReflectionCubeMap::FDynamicReflectionCubeMap()
    : Super()
{
    
}

void FDynamicReflectionCubeMap::UpdateCalculations(float deltaTime, const FViewportInfo& inViewportInfo)
{
    if (viewports.size() >= 6)
    {
        for (int i = 0; i < geometryMap->GetDynamicReflectionMeshObjectNumber(); i++)
        {
            CMeshComponent* meshComponent = geometryMap->GetDynamicReflectionComponent(i);
            XMFLOAT3 position = meshComponent->GetPosition();
            SetViewportPosition(fvector_3d(position.x, position.y, position.z));
            
            for (int viewportIndex = 0; viewportIndex < 6; viewportIndex++)
            {
                FViewportInfo viewportInfo;
                XMFLOAT3 viewportPosition = viewports[viewportIndex]->GetPosition();
                viewportInfo.viewWorldPosition = XMFLOAT4(viewportPosition.x, viewportPosition.y, viewportPosition.z, 1.f);
                viewportInfo.viewMatrix = viewports[viewportIndex]->viewMatrix;
                viewportInfo.projectMatrix = viewports[viewportIndex]->projectMatrix;
        
                geometryMap->UpdateCalculationsViewport(
                    deltaTime,
                    viewportInfo,
                    i * 6 + viewportIndex
                    + 1
                );
            }
        }
    }
}

void FDynamicReflectionCubeMap::Init(
    FGeometryMap* inGeometryMap
    , FDirectXPipelineState* inDirectXPipelineState
    , FRenderLayerManager* inRenderLayerManager
)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FDynamicReflectionCubeMap::PreDraw(float deltaTime)
{
    if (FCubeMapRenderTarget* cubemapRT = dynamic_cast<FCubeMapRenderTarget*>(renderTarget.get()))
    {
        for (int i = 0; i < geometryMap->GetDynamicReflectionMeshObjectNumber(); i++)
        {
            //指向那个资源 转换状态
            CD3DX12_RESOURCE_BARRIER resourceBarrierPresent = 
                CD3DX12_RESOURCE_BARRIER::Transition(
                    renderTarget->GetRenderTaget(),
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    D3D12_RESOURCE_STATE_RENDER_TARGET
                );

            GetGraphicsCommandList()->ResourceBarrier(1, &resourceBarrierPresent);

            //需要每帧执行,绑定矩形框
            D3D12_VIEWPORT rtViewPort = renderTarget->GetViewport();
            D3D12_RECT rtScissorRectt = renderTarget->GetScissorRect();
            GetGraphicsCommandList()->RSSetViewports(1, &rtViewPort);
            GetGraphicsCommandList()->RSSetScissorRects(1, &rtScissorRectt);
        
            UINT cbvSize = geometryMap->GetViewportConstantBufferByteSize();
            
            //清除画布
            for (size_t viewportIndex = 0; viewportIndex < 6; viewportIndex++)
            {
                GetGraphicsCommandList()->ClearRenderTargetView(
                    cubemapRT->cpuRenderTargetView[viewportIndex],
                    DirectX::Colors::CadetBlue,
                    0,
                    nullptr
                    );

                //清除深度模板缓存
                GetGraphicsCommandList()->ClearDepthStencilView(
                    dsvDesc,
                    D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
                    1.f,
                    0,
                    0,
                    NULL
                );

                GetGraphicsCommandList()->OMSetRenderTargets(
                    1,
                    &cubemapRT->cpuRenderTargetView[viewportIndex],
                    true,
                    &dsvDesc
                );
            
                //更新/绑定6个摄像机
                D3D12_GPU_VIRTUAL_ADDRESS gpuViewportAddress = geometryMap->GetViewportConstantBufferViewGPUVirtualAddress();
                gpuViewportAddress += (1 + i * 6 + viewportIndex) * cbvSize;
                GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, gpuViewportAddress);
            
                //各类层级渲染
                renderLayerManager->Draw(RENDERLAYER_BACKGROUND, deltaTime);
                renderLayerManager->Draw(RENDERLAYER_OPAQUE, deltaTime);
                renderLayerManager->Draw(RENDERLAYER_ALPHATEST, deltaTime);
                renderLayerManager->Draw(RENDERLAYER_TRANSPARENT, deltaTime);
            }

            //指向那个资源 转换其状态
            CD3DX12_RESOURCE_BARRIER resourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
                renderTarget->GetRenderTaget(),
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                D3D12_RESOURCE_STATE_GENERIC_READ
            );
            GetGraphicsCommandList()->ResourceBarrier(1, &resourceBarrierPresentRenderTarget);
            
            StartSetMainViewportRenderTarget();
            
            geometryMap->DrawViewport(deltaTime);
            
            //更新Cubemap
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
                6,
                cubemapRT->gpuShaderResourceView
            );
            
            Draw(deltaTime);
            
            renderLayerManager->FindObjDraw(
                RENDERLAYER_OPAQUE_REFLECTOR,
                deltaTime,
                geometryMap->GetDynamicReflectionComponent(i)
            );
            
            //重置Cubemap
            geometryMap->DrawTextureCubemap(deltaTime);
            
            EndSetMainViewportRenderTarget();
        }
            
    }
}

void FDynamicReflectionCubeMap::Draw(float deltaTime)
{
    
}

bool FDynamicReflectionCubeMap::IsExistDynamicReflectionMesh()
{
    return geometryMap->GetDynamicReflectionMeshObjectNumber() > 0;
}

void FDynamicReflectionCubeMap::BuildDepthStencilDescriptor()
{
    UINT descriptorHandleIncrementSize = GetDescriptorHandleIncrementSizeByDSV();
    dsvDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
        GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
        1,
        descriptorHandleIncrementSize
    );
}

void FDynamicReflectionCubeMap::BuildRenderTargetDescriptor()
{
    BuildRenderTargetRTV();
    
    BuildRenderTargetSRV();
    
    renderTarget->Init(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicReflectionCubeMap::BuildRenderTargetRTV()
{
    UINT rtvDescSize = GetDescriptorHandleIncrementSizeByRTV();
    UINT cbvDescSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
    
    //RTV起始地址
    D3D12_CPU_DESCRIPTOR_HANDLE rtvDesAddr = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

    if (FCubeMapRenderTarget* cubemapRT = dynamic_cast<FCubeMapRenderTarget*>(renderTarget.get()))
    {
        //偏移的地址记录
        for (int i = 0; i < 6; i++)
        {
            cubemapRT->cpuRenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
                rtvDesAddr,
                FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,
                rtvDescSize
            );
        }
    }
    
}

void FDynamicReflectionCubeMap::BuildRenderTargetSRV()
{
    
    if (FCubeMapRenderTarget* cubemapRT = dynamic_cast<FCubeMapRenderTarget*>(renderTarget.get()))
    {
        D3D12_CPU_DESCRIPTOR_HANDLE cpuRTVDesAddr = geometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuSRVDesAddr = geometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
        
        UINT cbvDescSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
        cubemapRT->cpuShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            cpuRTVDesAddr,
            geometryMap->GetDrawTexture2DResourcesNumber() + geometryMap->GetDrawTextureCubemapResourcesNumber(),
            cbvDescSize
        );
    
        cubemapRT->gpuShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
            gpuSRVDesAddr,
            geometryMap->GetDrawTexture2DResourcesNumber() + geometryMap->GetDrawTextureCubemapResourcesNumber(),
            cbvDescSize
        );
    }
}
