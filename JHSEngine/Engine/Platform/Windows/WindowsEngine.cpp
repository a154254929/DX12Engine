#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"

#ifdef _WIN32
#include "WindowsMessageProcessing.h"

FWindowsEngine::FWindowsEngine()
    : m4xQualityLevels(0)
    , bMSAA4XEnabled(false)
    , backBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
    , depthStencilBufferFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
    , currentSwapBuffIndex(0)
{
    for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; ++i)
    {
        swapChainBuffer.push_back(ComPtr<ID3D12Resource>());
    }
}

int FWindowsEngine::PreInit(FWinMainCommandParameters inParameters)
{
    //日志系统初始化
    const char logPath[] = "../log";
    init_log_system(logPath);
    Engine_Log("Log Init.");
     
    //处理命令


    Engine_Log("Engine pre initialization complete.");
    return 0;
}

int FWindowsEngine::Init(FWinMainCommandParameters inParameters)
{


    if (InitWindows(inParameters))
    {

    }

    if (InitDirect3D())
    {

    }
    Engine_Log("Engine initialization complete.");
    return 0;
}

int FWindowsEngine::PostInit()
{
    ANALYSIS_HRESULT(graphicsCommandList->Reset(commandAllocator.Get(), NULL));
    for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; ++i)
    {
        swapChainBuffer[i].Reset();
    }
    depthStencilBuffer.Reset();

    swapChain->ResizeBuffers(
        FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
        FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
        FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
        backBufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    );

    //拿到描述Size
    rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE heapHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; ++i)
    {
        swapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainBuffer[i]));
        d3dDevice->CreateRenderTargetView(
            swapChainBuffer[i].Get(),
            nullptr,
            heapHandle
        );
        heapHandle.Offset(1, rtvDescriptorSize);
    }

    D3D12_RESOURCE_DESC resourceDesv;
    resourceDesv.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    resourceDesv.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    resourceDesv.Alignment = 0;
    resourceDesv.MipLevels = 1;
    resourceDesv.DepthOrArraySize = 1;
    resourceDesv.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resourceDesv.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
    resourceDesv.SampleDesc.Quality = bMSAA4XEnabled ? (m4xQualityLevels - 1) : 0;
    resourceDesv.Format = DXGI_FORMAT_R24G8_TYPELESS;
    resourceDesv.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    resourceDesv.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    D3D12_CLEAR_VALUE clearValue;
    clearValue.DepthStencil.Depth = 1.f;
    clearValue.DepthStencil.Stencil = 0;
    clearValue.Format = depthStencilBufferFormat;

    CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    d3dDevice->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesv,
        D3D12_RESOURCE_STATE_COMMON,
        &clearValue,
        IID_PPV_ARGS(depthStencilBuffer.GetAddressOf())
    );

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = depthStencilBufferFormat;
    dsvDesc.Texture2D.MipSlice = 0;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    d3dDevice->CreateDepthStencilView(
        depthStencilBuffer.Get(),
        &dsvDesc,
        dsvHeap->GetCPUDescriptorHandleForHeapStart()
    );

    CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        depthStencilBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE
    );
    graphicsCommandList->ResourceBarrier(1, &ResourceBarrier);
    graphicsCommandList->Close();

    ID3D12CommandList* commandList[] = { graphicsCommandList.Get()};
    commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

    //覆盖原先Window画布
    //描述视口尺寸
    viewprotInfo.TopLeftX = 0;
    viewprotInfo.TopLeftY = 0;
    viewprotInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    viewprotInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    viewprotInfo.MinDepth = 0.f;
    viewprotInfo.MaxDepth = 1.f;

    //矩形
    viewprotRect.left = 0;
    viewprotRect.top = 0;
    viewprotRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    viewprotRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

    WaitGPUCommandQueueComplete();

    Engine_Log("Engine post initialization complete.");
    return 0;
}

void FWindowsEngine::Tick()
{
    ANALYSIS_HRESULT(commandAllocator->Reset());

    ANALYSIS_HRESULT(graphicsCommandList->Reset(commandAllocator.Get(), NULL));

    CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    graphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

    //
    //
    graphicsCommandList->RSSetViewports(1, &viewprotInfo);
    graphicsCommandList->RSSetScissorRects(1, &viewprotRect);

    //Çå³ý»­²¼
    graphicsCommandList->ClearRenderTargetView(GetCurrentSwapBufferView(),
        DirectX::Colors::Red,
        0, nullptr);

    //Çå³ýÉî¶ÈÄ£°å»º³åÇø
    graphicsCommandList->ClearDepthStencilView(GetCurrentDepthStencilView(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
        1.f, 0, 0, NULL);

    //
    D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
    graphicsCommandList->OMSetRenderTargets(1, &SwapBufferView,
        true, &DepthStencilView);

    CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    graphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

    //Â¼ÈëÍê³É
    ANALYSIS_HRESULT(graphicsCommandList->Close());

    //Ìá½»ÃüÁî
    ID3D12CommandList* CommandList[] = { graphicsCommandList.Get() };
    commandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

    //½»»»Á½¸öbuff»º³åÇø
    ANALYSIS_HRESULT(swapChain->Present(0, 0));
    currentSwapBuffIndex = !(bool)currentSwapBuffIndex;

    //CPUµÈGPU
    WaitGPUCommandQueueComplete();
}

int FWindowsEngine::PreExit()
{
    Engine_Log("Engine pre exit complete.");
    return 0;
}

int FWindowsEngine::Exit()
{
    Engine_Log("Engine exit complete.");
    return 0;
}

int FWindowsEngine::PostExit()
{
    FEngineRenderConfig::Destroy();
    Engine_Log("Engine post exit complete.");
    return 0;
}

ID3D12Resource* FWindowsEngine::GetCurrentSwapBuff() const
{
    return swapChainBuffer[currentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentSwapBufferView() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        currentSwapBuffIndex,
        rtvDescriptorSize
    );
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentDepthStencilView() const
{
    return dsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void FWindowsEngine::WaitGPUCommandQueueComplete()
{
    currentFenceIndex++;

    //
    ANALYSIS_HRESULT(commandQueue->Signal(fence.Get(), currentFenceIndex));

    if (fence->GetCompletedValue() < currentFenceIndex)
    {
        //
        //SECURITY_ATTRIBUTES
        //CREATE_EVENT_INITIAL_SET  0x00000002
        //CREATE_EVENT_MANUAL_RESET 0x00000001
        //ResetEvents
        HANDLE eventEX = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

        //GPU
        ANALYSIS_HRESULT(fence->SetEventOnCompletion(currentFenceIndex, eventEX));

        //
        WaitForSingleObject(eventEX, INFINITE);
        CloseHandle(eventEX);
    }
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
    //注册窗口
    WNDCLASSEX windowsClass;
    windowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
    windowsClass.cbClsExtra = 0;//是否需要额外空间
    windowsClass.cbWndExtra = 0;//是否需要额外内存
    windowsClass.hbrBackground = nullptr;//如果没有设置就是GDI擦除
    windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//设置一个箭头光标
    windowsClass.hIcon = nullptr;//应用程序方舟磁盘上显示的图标
    windowsClass.hIconSm = nullptr;//应用程序左上角图标
    windowsClass.hInstance = InParameters.HInstance;//窗口实例
    windowsClass.lpszClassName = L"JHSEngine";//窗口名字
    windowsClass.lpszMenuName = nullptr;//
    windowsClass.style = CS_VREDRAW | CS_HREDRAW;//怎么绘制窗口 水平或垂直
    windowsClass.lpfnWndProc = EngineWindowProc;//消息处理函数

    //注册在窗口
    ATOM registerAtom = RegisterClassEx(&windowsClass);

    if (!registerAtom)
    {
        Engine_Log_Error("Register Fail.");
        MessageBox(NULL, L"Register windows class fail", L"Error", MB_OK);
    }

    RECT rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };

    //lpRect 视口
    //dwStyle 视口风格
    //bMenu 没有菜单
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    mainWindowsHandle = CreateWindowEx(
        NULL,                       //窗口额外风格
        L"JHSEngine",               //窗口名称
        L"JHS Engine",              //窗口标题栏名称
        WS_OVERLAPPEDWINDOW,        //窗口风格
        100, 100,                   //窗口坐标
        windowWidth, windowHeight,  //´°¿Ú¿í¸ß
        NULL,                       //¸±´°¿Ú¾ä±ú
        nullptr,                    //²Ëµ¥¾ä±ú
        InParameters.HInstance,     //´°¿ÚÊµÀý
        NULL                        //¶îÍâ²ÎÊý
    );

    if (!mainWindowsHandle)
    {
        Engine_Log_Error("InitWindow fail.");
        MessageBox(0, L"CreateWindow Failed", 0, 0);
        return false;
    }

    //ÏÔÊ¾´°¿Ú
    ShowWindow(mainWindowsHandle, SW_SHOW);
    UpdateWindow(mainWindowsHandle);

    Engine_Log("InitWindow complete.");
}

bool FWindowsEngine::InitDirect3D()
{

    ComPtr<ID3D12Debug> d3d12Debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12Debug))))
    {
        d3d12Debug->EnableDebugLayer();
    }

    //HRESULT
    //  S_OK            
    //  E_UNEXCEPTED        意外失败
    //  E_NOTIMPL           未实现
    //  E_INVALIDARG        未能分配所需的内存
    //  E_NOINTERFACE       不支持此接口
    //  E_POINTER           无效指针
    //  E_HANDLE            无效句柄
    //  E_ABORT             操作终止
    ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
    IID_PPV_ARGS(&d3dDevice);
    HRESULT d3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_10_0, IID_PPV_ARGS(&d3dDevice));
    if (FAILED(d3dDeviceResult))
    {
        //warp
        ComPtr<IDXGIAdapter> warpAdapter;
        ANALYSIS_HRESULT(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
        ANALYSIS_HRESULT(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice)));
    }

    //创建Fence对象,为CPU和GPU同步做准备
    //D3D12_FENCE_FLAG_NONE
    //D3D12_FENCE_FLAG_SHARED
    //D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER
    /*
    fence->SetEnventOnCompletion
    提交命令
    提交呈现
    Queue->Signal
    wait
    */
    ANALYSIS_HRESULT(d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    //初始化命令对象
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
    ANALYSIS_HRESULT(d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));

    ANALYSIS_HRESULT(d3dDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(commandAllocator.GetAddressOf()))
    );

    ANALYSIS_HRESULT(d3dDevice->CreateCommandList(
        0,//默认一个GPU
        D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
        commandAllocator.Get(),//将CommandList关联到分配器
        NULL,
        IID_PPV_ARGS(graphicsCommandList.GetAddressOf()))
    );

    graphicsCommandList->Close();
    //多重采样
    ////////////////////////////////////////////////////////////////////////////////
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels;
    qualityLevels.Format = backBufferFormat;
    qualityLevels.SampleCount = 4;
    qualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    qualityLevels.NumQualityLevels = 0;

    ANALYSIS_HRESULT(d3dDevice->CheckFeatureSupport(
        D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &qualityLevels,
        sizeof(qualityLevels)
    ));

    m4xQualityLevels = qualityLevels.NumQualityLevels;

    //交换链
    ////////////////////////////////////////////////////////////////////////////////
    swapChain.Reset();
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    swapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Format = backBufferFormat;

    swapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或者资源作为输出渲染目标
    swapChainDesc.OutputWindow = mainWindowsHandle;//指定Window句柄
    swapChainDesc.Windowed = true;//以窗口运行
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    swapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
    swapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (m4xQualityLevels - 1) : 0;

    ANALYSIS_HRESULT(dxgiFactory->CreateSwapChain(commandQueue.Get(), &swapChainDesc, swapChain.GetAddressOf()));

    //资源描述对象
    ////////////////////////////////////////////////////////////////////////////////
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc;
    rtvDescriptorHeapDesc.NumDescriptors = 2;
    /*
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV  CBV常量缓冲区视图 SRV着色器资源视图 UAV无需访问视图
        D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER      采样器视图
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV          渲染目标视图
        D3D12_DESCRIPTOR_HEAP_TYPE_DSV          深度/蒙版视图资源
    */
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDescriptorHeapDesc.NodeMask = 0;
    ANALYSIS_HRESULT(d3dDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(rtvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc;
    dsvDescriptorHeapDesc.NumDescriptors = 1;
    dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvDescriptorHeapDesc.NodeMask = 0;
    ANALYSIS_HRESULT(d3dDevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(dsvHeap.GetAddressOf())));

    return false;
}
#endif
