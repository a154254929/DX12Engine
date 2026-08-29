#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../Engine/Mesh/Core/MeshManager.h"
#ifdef _WIN32
#include "../Platform/Windows/WindowsEngine.h"
#else
#include "../Core/Engine.h"
#endif

void IDirectXDeviceInterface::StartSetMainViewportRenderTarget()
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            inEngine->GetRenderingEngine()->StartSetMainViewportRenderTarget();
        }
    }
}

void IDirectXDeviceInterface::EndSetMainViewportRenderTarget()
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            inEngine->GetRenderingEngine()->EndSetMainViewportRenderTarget();
        }
    }
}

void IDirectXDeviceInterface::ClearMainViewportSwapChainCanvas()
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            inEngine->GetRenderingEngine()->ClearMainViewportSwapChainCanvas();
        }
    }
}

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->fence;
        }
    }
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->d3dDevice;
        }
    }
}

CLightManager* IDirectXDeviceInterface::GetLightManager() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->GetLightManager();
        }
    }
    return NULL;
}

CMeshManager* IDirectXDeviceInterface::GetMeshManager() const
{
    return GetEngine()->GetMeshManager();
}

CWorld* IDirectXDeviceInterface::GetWorld() const
{
    if (CWindowsEngine* inEngine = GetEngine())
    {
        return inEngine->GetWorld();
    }
    
    return NULL;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->commandQueue;
        }
    }
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->commandAllocator;
        }
    }
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->graphicsCommandList;
        }
    }
}

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetRTVHeap() const
{
    if (CWindowsEngine* inEngine = GetEngine())
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->rtvHeap.Get();
        }
    }
    return nullptr;
}

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetDSVHeap() const
{
    if (CWindowsEngine* inEngine = GetEngine())
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->dsvHeap.Get();
        }
    }
    return nullptr;
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByDSV() const
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByRTV() const
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->GetRenderingEngine()->currentFenceIndex;
        }
    }
    return 0;
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        if (inEngine->GetRenderingEngine())
        {
            return inEngine->mainWindowsHandle;
        }
    }
    return HWND();
}

FRenderingPipeline* IDirectXDeviceInterface::GetRenderingPipeline() const
{   
    if (CMeshManager* meshManager = GetMeshManager())
    {
        return meshManager->GetRenderingPipeline();
    }
    return NULL;
}

FRenderLayerManager* IDirectXDeviceInterface::GetRenderingLayerManager() const
{
    if (FRenderingPipeline* renderingPipeline = GetRenderingPipeline())
    {
        return renderingPipeline->GetRenderingLayerManager();
    }
    return NULL;
}

#ifdef _WIN32
CWindowsEngine* IDirectXDeviceInterface::GetEngine() const
{
    return dynamic_cast<CWindowsEngine*>(engine);
}
#else
CEngine* IDirectXDeviceInterface::GetEngine() const
{
    return engine;
}
#endif

#if EDITOR_ENGINE
#include "../../EditorEngine/EditorEngine.h"
CEditorEngine* IDirectXDeviceInterface::GetEditorEngine() const
{
#ifdef _WIN32
    if (CWindowsEngine* inEngine = GetEngine())
#else
    if (CEngine* inEngine = GetEngine())
#endif
    {
        return inEngine->editorEngine;
    }
    return nullptr;
}
#endif

ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence() const
{
    return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice() const
{
    return Interface.GetD3dDevice();
}

CMeshManager* IDirectXDeviceInterface_Struct::GetMeshManager() const
{
    return Interface.GetMeshManager();
}

CWorld* IDirectXDeviceInterface_Struct::GetWorld() const
{
    return Interface.GetWorld();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue() const
{
    return Interface.GetCommandQueue();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator() const
{
    return Interface.GetCommandAllocator();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList() const
{
    return Interface.GetGraphicsCommandList();
}

ID3D12DescriptorHeap* IDirectXDeviceInterface_Struct::GetRTVHeap() const
{
    return Interface.GetRTVHeap();
}

ID3D12DescriptorHeap* IDirectXDeviceInterface_Struct::GetDSVHeap() const
{
    return Interface.GetDSVHeap();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByDSV() const
{
    return Interface.GetDescriptorHandleIncrementSizeByDSV();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByRTV() const
{
    return Interface.GetDescriptorHandleIncrementSizeByRTV();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const
{
    return Interface.GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex() const
{
    return Interface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle() const
{
    return Interface.GetMainWindowsHandle();
}

FRenderingPipeline* IDirectXDeviceInterface_Struct::GetRenderingPipeline() const
{
    return Interface.GetRenderingPipeline();
}

FRenderLayerManager* IDirectXDeviceInterface_Struct::GetRenderingLayerManager() const
{
    return Interface.GetRenderingLayerManager();
}

#ifdef _WIN32
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine() const
{
    return Interface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterface_Struct::GetEngine() const
{
    return Interface.GetEngine();
}
#endif

#if EDITOR_ENGINE
CEditorEngine* IDirectXDeviceInterface_Struct::GetEditorEngine() const
{
    return Interface.GetEditorEngine();
}
#endif
