#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Mesh/BoxMesh.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CWindowsEngine::CWindowsEngine()
    : currentFenceIndex(0)
    , m4xQualityLevels(0)
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

CWindowsEngine::~CWindowsEngine()
{

}

int CWindowsEngine::PreInit(FWinMainCommandParameters inParameters)
{
    //日志系统初始化
    const char logPath[] = "../log";
    init_log_system(logPath);
    Engine_Log("Log Init.");
     
    //处理命令


    Engine_Log("Engine pre initialization complete.");
    return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters inParameters)
{
    InitWindows(inParameters);

    InitDirect3D();

    PostInitDirect3D();

    Engine_Log("Engine initialization complete.");
    return 0;
}

int CWindowsEngine::PostInit()
{
    ANALYSIS_HRESULT(graphicsCommandList->Reset(commandAllocator.Get(), NULL));
    {
        //构建Mesh
        CBoxMesh* boxMesh = CBoxMesh::CreateMesh();
	}

	ANALYSIS_HRESULT(graphicsCommandList->Close());

    ID3D12CommandList* commandList[] = { graphicsCommandList.Get() };
    commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

	WaitGPUCommandQueueComplete();
	Engine_Log("Engine post initialization complete.");
	return 0;
}

void CWindowsEngine::Tick(float deltaTime)
{
    //重置录制相关的内存,为下一帧做准备
    ANALYSIS_HRESULT(commandAllocator->Reset());

    for (auto& temp : IRenderingInterface::renderingInterfaces)
    {
        temp->PreDraw(deltaTime);
    }

    //指向那个资源 转换状态
    CD3DX12_RESOURCE_BARRIER resourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
        GetCurrentSwapBuff(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

	graphicsCommandList->ResourceBarrier(1, &resourceBarrierPresent);

    //需要每帧执行,绑定矩形框
    graphicsCommandList->RSSetViewports(1, &viewportInfo);
    graphicsCommandList->RSSetScissorRects(1, &viewportRect);
    //清除画布
    graphicsCommandList->ClearRenderTargetView(
        GetCurrentSwapBufferView(),
        DirectX::Colors::CadetBlue,
        0,
        nullptr
    );

    //清除深度模板缓存
    graphicsCommandList->ClearDepthStencilView(
        GetCurrentDepthStencilView(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
        1.f,
        0,
        0,
        NULL
    );

    //输出的合并阶段
    D3D12_CPU_DESCRIPTOR_HANDLE swapBufferView = GetCurrentSwapBufferView();
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GetCurrentDepthStencilView();
    graphicsCommandList->OMSetRenderTargets(
        1,
        &swapBufferView,
        true,
        &depthStencilView
    );

    //渲染其他内容
    for (auto& temp : IRenderingInterface::renderingInterfaces)
    {
        temp->Draw(deltaTime);
        temp->PostDraw(deltaTime);
    }

    //指向那个资源 转换其状态
    CD3DX12_RESOURCE_BARRIER resourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
        GetCurrentSwapBuff(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    graphicsCommandList->ResourceBarrier(1, &resourceBarrierPresentRenderTarget);

    //录入完成
    ANALYSIS_HRESULT(graphicsCommandList->Close());

    //提交命令
    ID3D12CommandList* commandList[] = { graphicsCommandList.Get() };
    commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

    //交换两个buff缓冲区
    ANALYSIS_HRESULT(swapChain->Present(0, 0));
    currentSwapBuffIndex ^= 1;

    //CPU等待GPU
    WaitGPUCommandQueueComplete();
}

int CWindowsEngine::PreExit()
{
    Engine_Log("Engine pre exit complete.");
    return 0;
}

int CWindowsEngine::Exit()
{
    Engine_Log("Engine exit complete.");
    return 0;
}

int CWindowsEngine::PostExit()
{
    FEngineRenderConfig::Destroy();
    Engine_Log("Engine post exit complete.");
    return 0;
}

ID3D12Resource* CWindowsEngine::GetCurrentSwapBuff() const
{
    return swapChainBuffer[currentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CWindowsEngine::GetCurrentSwapBufferView() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        currentSwapBuffIndex,
        rtvDescriptorSize
    );
}

D3D12_CPU_DESCRIPTOR_HANDLE CWindowsEngine::GetCurrentDepthStencilView() const
{
    return dsvHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT CWindowsEngine::GetDXGISampleCount() const
{
    return bMSAA4XEnabled ? 4 : 1;
}

UINT CWindowsEngine::GetDXGISampleQuality() const
{
    return bMSAA4XEnabled ? (m4xQualityLevels - 1) : 0;
}

void CWindowsEngine::WaitGPUCommandQueueComplete()
{
    currentFenceIndex++;

    //像GPU设置新的隔离点 等待GPU处理完信号
    ANALYSIS_HRESULT(commandQueue->Signal(fence.Get(), currentFenceIndex));

    if (fence->GetCompletedValue() < currentFenceIndex)
    {
        //创建或打开一个事件内核对象,并返回该对象的句柄
        //SECURITY_ATTRIBUTES
        //CREATE_EVENT_INITIAL_SET  0x00000002
        //CREATE_EVENT_MANUAL_RESET 0x00000001
        //ResetEvents
        HANDLE eventEX = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

        //GPU完成后会通知句柄
        ANALYSIS_HRESULT(fence->SetEventOnCompletion(currentFenceIndex, eventEX));

        //等待GPU,阻塞主线程
        WaitForSingleObject(eventEX, INFINITE);
        CloseHandle(eventEX);
    }
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
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

bool CWindowsEngine::InitDirect3D()
{
	//Debug
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
    HRESULT d3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice));
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

////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask Ö¸Ê¾¸ÃÃüÁî¶ÓÁÐÓ¦ÔÚÄÄ¸öGPU½ÚµãÉÏÖ´ÐÐ

    //初始化命令对象
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
    ANALYSIS_HRESULT(d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));

    ID3D12CommandAllocator Allocator();
    ANALYSIS_HRESULT(d3dDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(commandAllocator.GetAddressOf())
    ));

    ANALYSIS_HRESULT(d3dDevice->CreateCommandList(
        0,//默认一个GPU
        D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
        commandAllocator.Get(),//将CommandList关联到分配器
        NULL,//ID3D12PipelineState
        IID_PPV_ARGS(graphicsCommandList.GetAddressOf())
    ));

    ANALYSIS_HRESULT(graphicsCommandList->Close());
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
////////////////////////////////////////////////////////////////////
    swapChain.Reset();
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    swapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.Format = backBufferFormat;
    
	swapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	//DXGI_USAGE_BACK_BUFFER //
	//DXGI_USAGE_READ_ONLY 
	//DXGI_USAGE_SHADER_INPUT
	//DXGI_USAGE_SHARED
	//DXGI_USAGE_UNORDERED_ACCESS
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或者资源作为输出渲染目标
    swapChainDesc.OutputWindow = mainWindowsHandle;//指定Window句柄
    swapChainDesc.Windowed = true;//以窗口运行
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    swapChainDesc.SampleDesc.Count = GetDXGISampleCount();
    swapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();

    ANALYSIS_HRESULT(dxgiFactory->CreateSwapChain(
        commandQueue.Get(),
        &swapChainDesc,
        swapChain.GetAddressOf()
    ));

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

void CWindowsEngine::PostInitDirect3D()
{
    //同步
    WaitGPUCommandQueueComplete();

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
    resourceDesv.SampleDesc.Count = GetDXGISampleCount();
    resourceDesv.SampleDesc.Quality = GetDXGISampleQuality();
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

    CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        depthStencilBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE
    );
    graphicsCommandList->ResourceBarrier(1, &resourceBarrier);
    graphicsCommandList->Close();

    ID3D12CommandList* commandList[] = { graphicsCommandList.Get() };
    commandQueue->ExecuteCommandLists(_countof(commandList), commandList);

    //覆盖原先Window画布
    //描述视口尺寸
    viewportInfo.TopLeftX = 0;
    viewportInfo.TopLeftY = 0;
    viewportInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    viewportInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    viewportInfo.MinDepth = 0.f;
    viewportInfo.MaxDepth = 1.f;

    //矩形
    viewportRect.left = 0;
    viewportRect.top = 0;
    viewportRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
    viewportRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

    WaitGPUCommandQueueComplete();
    return;
}
#endif