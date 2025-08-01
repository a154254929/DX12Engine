﻿#pragma once

#ifdef _WIN32
#include "../../Core/Engine.h"

class FWindowsEngine :public FEngine
{
public:
    FWindowsEngine();
    ~FWindowsEngine();

    virtual int PreInit(FWinMainCommandParameters);
    virtual int Init(FWinMainCommandParameters);
    virtual int PostInit();

    virtual void Tick();

    virtual int PreExit();
    virtual int Exit();
    virtual int PostExit();

public:
    ID3D12Resource* GetCurrentSwapBuff() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;

protected:
    void WaitGPUCommandQueueComplete();

private:
    bool InitWindows(FWinMainCommandParameters InParameters);

    bool InitDirect3D();
protected:
    UINT64 currentFenceIndex;
    int currentSwapBuffIndex;

    ComPtr<IDXGIFactory4> dxgiFactory;	//创建DirectX图形基础结构(DXGI)对象
    ComPtr<ID3D12Device> d3dDevice;     //创建命令分配器,命令列表,命令队列,Fence,资源,管道状态对象,堆,根
    ComPtr<ID3D12Fence> fence;          //一个用于同步CPU和一个或多个GPU对象

    ComPtr<ID3D12CommandQueue> commandQueue;	                    //队列
    ComPtr<ID3D12CommandAllocator> commandAllocator;                //存储
    ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;          //命令列表

    ComPtr<IDXGISwapChain> swapChain;          //交换链

    //描述符对象和堆
    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    ComPtr<ID3D12DescriptorHeap> dsvHeap;

    vector<ComPtr<ID3D12Resource>> swapChainBuffer;
    ComPtr<ID3D12Resource> depthStencilBuffer;

    D3D12_VIEWPORT viewprotInfo;
    D3D12_RECT viewprotRect;
protected:
    HWND mainWindowsHandle;         //主窗口句柄
    UINT m4xQualityLevels;
    bool bMSAA4XEnabled;
    DXGI_FORMAT backBufferFormat;
    DXGI_FORMAT depthStencilBufferFormat;

    UINT rtvDescriptorSize;
};
#endif