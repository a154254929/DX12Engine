#include "DynamicShadowCubeMap.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"
#include "../../../../../../Manager/LightManager.h"
#include "../../../../../../Component/Light/PointLightComponent.h"
#include "../../RenderLayer/RenderLayerManager.h"


FDynamicShadowCubeMap::FTmpViewportCapture::FTmpViewportCapture()
{
}

FDynamicShadowCubeMap::FTmpViewportCapture::FTmpViewportCapture(const fvector_3d& inPosition)
{
    BuildViewPortCapture(inPosition);
}

void FDynamicShadowCubeMap::FTmpViewportCapture::BuildViewPortCapture(const fvector_3d& inPosition)
{
    targetPosition[0] = fvector_3d(inPosition.x + 1.0f, inPosition.y, inPosition.z);
    targetPosition[1] = fvector_3d(inPosition.x - 1.0f, inPosition.y, inPosition.z);
    targetPosition[2] = fvector_3d(inPosition.x, inPosition.y + 1.0f, inPosition.z);
    targetPosition[3] = fvector_3d(inPosition.x, inPosition.y - 1.0f, inPosition.z);
    targetPosition[4] = fvector_3d(inPosition.x, inPosition.y, inPosition.z + 1.0f);
    targetPosition[5] = fvector_3d(inPosition.x, inPosition.y, inPosition.z - 1.0f);
}

FDynamicShadowCubeMap::FDynamicShadowCubeMap()
    : Super()
{
    CreateRenderTarget<FCubeMapRenderTarget>();
}

void FDynamicShadowCubeMap::Init(
    FGeometryMap* inGeometryMap,
    FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager
)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FDynamicShadowCubeMap::UpdateCalculations(float deltaTime, const FViewportInfo& inViewportInfo)
{
    if (viewports.size() >= 6)
    {
        for (int i = 0; i < GetLightManager()->GetLights().size(); i++)
        {
            CMeshComponent* meshComponent = geometryMap->dynamicReflectionMeshComponents[i];
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

void FDynamicShadowCubeMap::PreDraw(float deltaTime)
{
    if (FCubeMapRenderTarget* cubemapRT = dynamic_cast<FCubeMapRenderTarget*>(renderTarget.get()))
    {
        for (int i = 0; i < GetLightManager()->GetLights().size(); i++)
        {
            CLightComponent* lightComp = GetLightManager()->GetLights()[i];
            if (lightComp->GetLightType() != ELightType::PointLight)
            {
                continue;
            }
            
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
                    cubemapRT->GetCPURenderTargetView(viewportIndex),
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
                    &cubemapRT->GetCPURenderTargetView(viewportIndex),
                    true,
                    &dsvDesc
                );
            
                //更新/绑定6个摄像机
                D3D12_GPU_VIRTUAL_ADDRESS gpuViewportAddress = geometryMap->GetViewportConstantBufferViewGPUVirtualAddress();
                gpuViewportAddress += (1 + i * 6 + viewportIndex) * cbvSize;
                GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, gpuViewportAddress);
                
                renderLayerManager->ResetPSO(RENDERLAYER_OPAQUE_SHADOW, EPipelineState::VientianeShadowShadow);
            
                //各类层级渲染
                renderLayerManager->DrawMesh(deltaTime, RENDERLAYER_OPAQUE, ERenderingConditions::RC_Shadow);
                renderLayerManager->DrawMesh(deltaTime, RENDERLAYER_TRANSPARENT, ERenderingConditions::RC_Shadow);
                renderLayerManager->DrawMesh(deltaTime, RENDERLAYER_OPAQUE_REFLECTOR, ERenderingConditions::RC_Shadow);
            }

            //指向那个资源 转换其状态
            CD3DX12_RESOURCE_BARRIER resourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
                renderTarget->GetRenderTaget(),
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                D3D12_RESOURCE_STATE_GENERIC_READ
            );
            GetGraphicsCommandList()->ResourceBarrier(1, &resourceBarrierPresentRenderTarget);
            
            //更新Cubemap
            //6是在PSO中绑定的静态Cubemap的槽位,debug用
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
                6,
                cubemapRT->GetGPUSRVOffset()
            );
            
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(
                8,
                cubemapRT->GetGPUSRVOffset()
            );
        }
            
    }
}

void FDynamicShadowCubeMap::Draw(float deltaTime)
{
    
}

void FDynamicShadowCubeMap::SetViewportPosition(const fvector_3d& inPosition)
{
    FTmpViewportCapture tmpCapture(inPosition);
    
    for (int i = 0; i < 6; i++)
    {
        viewports[i]->SetPosition(XMFLOAT3(inPosition.x, inPosition.y, inPosition.z));
        viewports[i]->LookAt(
            inPosition,
            tmpCapture.targetPosition[i],
            tmpCapture.up[i]
        );
        viewports[i]->BuildViewMatrix(0.016f);
    }
}

void FDynamicShadowCubeMap::BuildViewPort(const fvector_3d& inPosition)
{
    FTmpViewportCapture tmpCapture(inPosition);
    
    for (int i = 0; i < 6; i++)
    {
        GClientViewport* viewport = CreateObject<GClientViewport>(new GClientViewport());
        viewports.push_back(viewport);
        
        viewport->SetPosition(XMFLOAT3(inPosition.x, inPosition.y, inPosition.z));
        viewport->LookAt(
            inPosition,
            tmpCapture.targetPosition[i],
            tmpCapture.up[i]
        );
        viewport->SetFrustum(90.f, 1.f, 0.1f, 1000.f);
        
        viewport->BuildViewMatrix(0.016f);
    }
}

void FDynamicShadowCubeMap::BuildDepthStencil()
{
    D3D12_RESOURCE_DESC resourceDesc;
    resourceDesc.Width = width;
    resourceDesc.Height = height;
    resourceDesc.Alignment = 0;
    resourceDesc.MipLevels = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    D3D12_CLEAR_VALUE clearValue;
    clearValue.DepthStencil.Depth = 1.f;
    clearValue.DepthStencil.Stencil = 0;
    clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    GetD3dDevice()->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &clearValue,
        IID_PPV_ARGS(depthStencilBuffer.GetAddressOf())
    );

    GetD3dDevice()->CreateDepthStencilView(
        depthStencilBuffer.Get(),
        NULL,
        dsvDesc
    );

    CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        depthStencilBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE
    );
    GetGraphicsCommandList()->ResourceBarrier(1, &resourceBarrier);
}

void FDynamicShadowCubeMap::BuildDepthStencilDescriptor()
{
    UINT descriptorHandleIncrementSize = GetDescriptorHandleIncrementSizeByDSV();
    dsvDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
        GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
        1,
        descriptorHandleIncrementSize
    );
}

void FDynamicShadowCubeMap::BuildRenderTargetDescriptor()
{
    BuildRenderTargetRTV();
    
    BuildRenderTargetSRV();
    
    renderTarget->Init(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicShadowCubeMap::BuildRenderTargetRTV()
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

void FDynamicShadowCubeMap::BuildRenderTargetSRV()
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

