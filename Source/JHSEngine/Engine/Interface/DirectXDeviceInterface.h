#pragma once
#include "../Core/Engine.h"

#ifdef _WIN32;
class FRenderLayerManager;
class FRenderingPipeline;
class CWindowsEngine;
#else
class CEngine;
#endif
//提供渲染内容的接口
class CLightManager;
class CMeshManager;
class CWorld;

class IDirectXDeviceInterface
{
friend struct IDirectXDeviceInterface_Struct;
public:
    void StartSetMainViewportRenderTarget();
    void EndSetMainViewportRenderTarget();
    void ClearMainViewportSwapChainCanvas();
    
public:
    ComPtr<ID3D12Fence> GetFence() const;
    ComPtr<ID3D12Device> GetD3dDevice() const;
    CLightManager* GetLightManager() const;
    CMeshManager* GetMeshManager() const;
    CWorld* GetWorld() const;

    ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
    
    ID3D12DescriptorHeap* GetRTVHeap() const;
    ID3D12DescriptorHeap* GetDSVHeap() const;
    
    UINT GetDescriptorHandleIncrementSizeByDSV() const;
    UINT GetDescriptorHandleIncrementSizeByRTV() const;
    UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const;

    UINT64 GetCurrentFenceIndex() const;
    HWND GetMainWindowsHandle() const;
    
protected:
    FRenderingPipeline* GetRenderingPipeline() const;
    FRenderLayerManager* GetRenderingLayerManager() const;

public:
#ifdef _WIN32
    CWindowsEngine* GetEngine() const;
#else
    CEngine* GetEngine() const;
#endif

#if EDITOR_ENGINE
    class CEditorEngine* GetEditorEngine() const;
#endif
    
private:

};


struct IDirectXDeviceInterface_Struct
{
public:
    ComPtr<ID3D12Fence> GetFence() const;
    ComPtr<ID3D12Device> GetD3dDevice() const;
    CMeshManager* GetMeshManager() const;
    CWorld* GetWorld() const;

    ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
    
    ID3D12DescriptorHeap* GetRTVHeap() const;
    ID3D12DescriptorHeap* GetDSVHeap() const;
    
    UINT GetDescriptorHandleIncrementSizeByDSV() const;
    UINT GetDescriptorHandleIncrementSizeByRTV() const;
    UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV() const;

    UINT64 GetCurrentFenceIndex() const;
    HWND GetMainWindowsHandle() const;
protected:
    FRenderingPipeline* GetRenderingPipeline() const;
    FRenderLayerManager* GetRenderingLayerManager() const;
    
public:
#ifdef _WIN32
    CWindowsEngine* GetEngine() const;
#else
    CEngine* GetEngine() const;
#endif

#if EDITOR_ENGINE
    class CEditorEngine* GetEditorEngine() const;
#endif
    
private:
    IDirectXDeviceInterface Interface;
};