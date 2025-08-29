#pragma once
#include "../Core/Engine.h"

#ifdef _WIN32
class CWindowsEngine;
#else
class CEngine;
#endif
//提供渲染内容的接口
class IDirectXDeviceInterface
{
public:
    ComPtr<ID3D12Fence> GetFence();
    ComPtr<ID3D12Device> GetD3dDevice();

    ComPtr<ID3D12CommandQueue> GetCommandQueue();
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();

    UINT64 GetCurrentFenceIndex();
    HWND GetMainWindowsHandle();

#ifdef _WIN32
    CWindowsEngine* GetEngine();
#else
    CEngine* GetEngine();
#endif
private:

};


struct IDirectXDeviceInterface_Struct
{
public:
    ComPtr<ID3D12Fence> GetFence();
    ComPtr<ID3D12Device> GetD3dDevice();

    ComPtr<ID3D12CommandQueue> GetCommandQueue();
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();

    UINT64 GetCurrentFenceIndex();
    HWND GetMainWindowsHandle();

#ifdef _WIN32
    CWindowsEngine* GetEngine();
#else
    CEngine* GetEngine();
#endif

private:
    IDirectXDeviceInterface Interface;
};