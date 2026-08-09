#include "DynamicCubeMap.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"
#include "../../../../../../Core/Viewport/ClientViewport.h"


FDynamicCubeMap::FTmpViewportCapture::FTmpViewportCapture()
{
}

FDynamicCubeMap::FTmpViewportCapture::FTmpViewportCapture(const fvector_3d& inPosition)
{
    BuildViewPortCapture(inPosition);
}

void FDynamicCubeMap::FTmpViewportCapture::BuildViewPortCapture(const fvector_3d& inPosition)
{
    targetPosition[0] = fvector_3d(inPosition.x + 1.0f, inPosition.y, inPosition.z);
    targetPosition[1] = fvector_3d(inPosition.x - 1.0f, inPosition.y, inPosition.z);
    targetPosition[2] = fvector_3d(inPosition.x, inPosition.y + 1.0f, inPosition.z);
    targetPosition[3] = fvector_3d(inPosition.x, inPosition.y - 1.0f, inPosition.z);
    targetPosition[4] = fvector_3d(inPosition.x, inPosition.y, inPosition.z + 1.0f);
    targetPosition[5] = fvector_3d(inPosition.x, inPosition.y, inPosition.z - 1.0f);
}

FDynamicCubeMap::FDynamicCubeMap()
    : Super()
{
    CreateRenderTarget<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::Init(
    FGeometryMap* inGeometryMap,
    FDirectXPipelineState* inDirectXPipelineState,
    FRenderLayerManager* inRenderLayerManager
)
{
    Super::Init(inGeometryMap, inDirectXPipelineState, inRenderLayerManager);
}

void FDynamicCubeMap::BuildViewPort(const fvector_3d& inPosition)
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

void FDynamicCubeMap::BuildDepthStencil()
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

void FDynamicCubeMap::SetViewportPosition(const fvector_3d& inPosition)
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

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
    BuildRenderTargetRTV();
    
    BuildRenderTargetSRV();
    
    renderTarget->Init(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
}
