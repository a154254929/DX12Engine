#include "NormalBuffer.h"

#include "Engine/Component/Mesh/Core/MeshComponentType.h"
#include "../Geometry/GeometryMap.h"
#include "../RenderLayer/RenderLayerManager.h"
#include "../RenderTarget/BufferRenderTarget.h"
#include "Engine/Config/EngineRenderConfig.h"

FNormalBuffer::FNormalBuffer()
{
    format = DXGI_FORMAT_R16G16B16A16_FLOAT;
}

void FNormalBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FNormalBuffer::Init(int inWidth, int inHeight)
{
    Super::Init(inWidth, inHeight);
}

void FNormalBuffer::Draw(float deltaTime)
{
    if (FBufferRenderTarget* bufferRenderTarget = dynamic_cast<FBufferRenderTarget*>(renderTarget.get()))
    {
        auto renderTargetViewport = renderTarget->GetViewport();
        auto renderTargetScissorRect = renderTarget->GetScissorRect();
        
        GetGraphicsCommandList()->RSSetViewports(1, &renderTargetViewport);
        GetGraphicsCommandList()->RSSetScissorRects(1, &renderTargetScissorRect);
        
        //指向那个资源 转换状态
        CD3DX12_RESOURCE_BARRIER resourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
            renderTarget->GetRenderTaget(),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            D3D12_RESOURCE_STATE_RENDER_TARGET
            );
        
        auto depthStencilView = GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
        const float defaultClearColor[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
        GetGraphicsCommandList()->ClearRenderTargetView(
            bufferRenderTarget->GetCPURenderTargetView(),
            defaultClearColor,
            0,
            nullptr
        );
        
        GetGraphicsCommandList()->ClearDepthStencilView(
            depthStencilView,
            D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
            1.f,
            0,
            0,
            nullptr
        );
        
        GetGraphicsCommandList()->OMSetRenderTargets(
            1,
            &bufferRenderTarget->GetCPURenderTargetView(),
            true,
            &depthStencilView
        );
        
        //从主视口渲染
        geometryMap->DrawViewport(deltaTime);
        
        renderLayerManager->ResetPSO(EMeshRenderLayerType::RENDERLAYER_NORMAL_BUFFER);
        renderLayerManager->DrawMesh(deltaTime, RENDERLAYER_OPAQUE, ERenderingConditions::RC_Shadow);
        renderLayerManager->DrawMesh(deltaTime, RENDERLAYER_TRANSPARENT, ERenderingConditions::RC_Shadow);
        renderLayerManager->DrawMesh(deltaTime, RENDERLAYER_OPAQUE_REFLECTOR, ERenderingConditions::RC_Shadow);
        
        //指向那个资源 转换其状态
        CD3DX12_RESOURCE_BARRIER resourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
            renderTarget->GetRenderTaget(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_GENERIC_READ
        );
    }
}

void FNormalBuffer::BuildDescriptors()
{
}

void FNormalBuffer::BuildRenderTargetRTV()
{
    UINT rtvDescriptorSize = GetDescriptorHandleIncrementSizeByRTV();
    auto rtvDesHeapStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
    int offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount + 6 + 6; //交换链+实时cubemap+ShadowCubemap
    if (FBufferRenderTarget* bufferRenderTarget = dynamic_cast<FBufferRenderTarget*>(renderTarget.get()))
    {
        bufferRenderTarget->GetCPURenderTargetView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDesHeapStart, offset, rtvDescriptorSize);
    }
}

void FNormalBuffer::BuildSRVDescriptors()
{
}

void FNormalBuffer::BuildRTVDescriptors()
{
    if (FBufferRenderTarget* bufferRenderTarget = dynamic_cast<FBufferRenderTarget*>(renderTarget.get()))
    {
        D3D12_RENDER_TARGET_VIEW_DESC rtvDescriptor = {};
        rtvDescriptor.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtvDescriptor.Format = format;
        rtvDescriptor.Texture2D.MipSlice = 0;
        rtvDescriptor.Texture2D.PlaneSlice = 0;
        
        GetD3dDevice()->CreateRenderTargetView(
            bufferRenderTarget->GetRenderTaget(),
            &rtvDescriptor,
            bufferRenderTarget->GetCPURenderTargetView()
        );
    }
}

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& outResource)
{
    CD3DX12_RESOURCE_DESC heapBufferDesc;
    memset(&heapBufferDesc, 0, sizeof(heapBufferDesc));
    heapBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    heapBufferDesc.Alignment = 0;
    heapBufferDesc.Width = width;
    heapBufferDesc.Height = height;
    heapBufferDesc.DepthOrArraySize = 1;
    heapBufferDesc.Format = format;
    heapBufferDesc.MipLevels = 1;
    heapBufferDesc.SampleDesc.Count = 1;
    heapBufferDesc.SampleDesc.Quality = 0;
    heapBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    heapBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    
    CD3DX12_HEAP_PROPERTIES heapBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    
    const float defaultClearColor[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
    CD3DX12_CLEAR_VALUE clearValue(format, defaultClearColor);
    
    GetD3dDevice()->CreateCommittedResource(
        &heapBufferProperties,
        D3D12_HEAP_FLAG_NONE,
        &heapBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        &clearValue,
        IID_PPV_ARGS(outResource.GetAddressOf())
    );
}
