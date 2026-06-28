#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#ifdef _WIN32
#include "../Platform/Windows/WindowsEngine.h"
#else
#include "../Core/Engine.h"
#endif

void IDirectXDeviceInterface::StartSetMainViewportRenderTarget()
{
}

void IDirectXDeviceInterface::EndSetMainViewportRenderTarget()
{
}

void IDirectXDeviceInterface::ClearMainViewportSwapChainCanvas()
{
}

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence()
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

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice()
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

CLightManager* IDirectXDeviceInterface::GetLightManager()
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

CMeshManager* IDirectXDeviceInterface::GetMeshManager()
{
    return GetEngine()->GetMeshManager();
}

CWorld* IDirectXDeviceInterface::GetWorld()
{
    if (CWindowsEngine* inEngine = GetEngine())
    {
        return inEngine->GetWorld();
    }
    
    return NULL;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
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

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
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

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList()
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

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetRTVHeap()
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

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetDSVHeap()
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

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByDSV()
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByRTV()
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
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
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
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

#ifdef _WIN32
CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
    return dynamic_cast<CWindowsEngine*>(engine);
}
#else
CEngine* IDirectXDeviceInterface::GetEngine()
{
    return engine;
}
#endif

ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence()
{
    return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice()
{
    return Interface.GetD3dDevice();
}

CMeshManager* IDirectXDeviceInterface_Struct::GetMeshManager()
{
    return Interface.GetMeshManager();
}

CWorld* IDirectXDeviceInterface_Struct::GetWorld()
{
    return Interface.GetWorld();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue()
{
    return Interface.GetCommandQueue();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator()
{
    return Interface.GetCommandAllocator();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList()
{
    return Interface.GetGraphicsCommandList();
}

ID3D12DescriptorHeap* IDirectXDeviceInterface_Struct::GetRTVHeap()
{
    return Interface.GetRTVHeap();
}

ID3D12DescriptorHeap* IDirectXDeviceInterface_Struct::GetDSVHeap()
{
    return Interface.GetDSVHeap();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByDSV()
{
    return Interface.GetDescriptorHandleIncrementSizeByDSV();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByRTV()
{
    return Interface.GetDescriptorHandleIncrementSizeByRTV();
}

UINT IDirectXDeviceInterface_Struct::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()
{
    return Interface.GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex()
{
    return Interface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle()
{
    return Interface.GetMainWindowsHandle();
}

#ifdef _WIN32
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
    return Interface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
    return Interface.GetEngine();
}
#endif