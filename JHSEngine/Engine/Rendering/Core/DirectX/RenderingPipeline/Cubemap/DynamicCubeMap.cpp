#include "DynamicCubeMap.h"
#include "../Geometry/GeometryMap.h"
#include "../PipelineState/DirectXPipelineState.h"
#include "../RenderTarget/CubeMapRenderTarget.h"
#include "../../../../../Core/Viewport/ClientViewport.h"


FDynamicCubeMap::FDynamicCubeMap()
    : geometryMap(NULL)
    , directXPipelineState(NULL)
    , width(512U)
    , height(512U)
{
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
    geometryMap = inGeometryMap;
    directXPipelineState = inDirectXPipelineState;
}

void FDynamicCubeMap::BuildViewPort(const fvector_3d& inPosition)
{
    struct FTmpViewportCapture
    {
        fvector_3d targetPosition[6];
        fvector_3d up[6] = {
            fvector_3d(0, 1, 0),
            fvector_3d(0, 1, 0),
            fvector_3d(0, 0, -1),
            fvector_3d(0, 0, 1),
            fvector_3d(0, 1, 0),
        };
    };
    
    FTmpViewportCapture tmpCapture;
    
    tmpCapture.targetPosition[0] = fvector_3d(inPosition.x + 1.0f, inPosition.y, inPosition.z);
    tmpCapture.targetPosition[1] = fvector_3d(inPosition.x - 1.0f, inPosition.y, inPosition.z);
    tmpCapture.targetPosition[2] = fvector_3d(inPosition.x, inPosition.y + 1.0f, inPosition.z);
    tmpCapture.targetPosition[3] = fvector_3d(inPosition.x, inPosition.y - 1.0f, inPosition.z);
    tmpCapture.targetPosition[4] = fvector_3d(inPosition.x, inPosition.y, inPosition.z + 1.0f);
    tmpCapture.targetPosition[5] = fvector_3d(inPosition.x, inPosition.y, inPosition.z - 1.0f);
    
    for (int i = 0; i < 6; i++)
    {
        GClientViewport* viewport = CreateObject<GClientViewport>(new GClientViewport());
        viewports.push_back(viewport);
        
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
