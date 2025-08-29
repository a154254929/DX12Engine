#include "WindowsEngine.h"

#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Core/World.h"
#include "../../Core/Camera.h"
#include "../../Rendering/Engine/DirectX/DirectX12RenderingEngine.h"

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
    : renderingEngine(new CDirectX12RenderingEngine())
{
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

    //渲染引擎初始化
    renderingEngine->PreInit(inParameters);
    return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters inParameters)
{
    InitWindows(inParameters);

    renderingEngine->SetMainWindowsHandle(mainWindowsHandle);

    renderingEngine->Init(inParameters);

    world = CreateObject<CWorld>(new CWorld());

    Engine_Log("Engine initialization complete.");
    return 0;
}

int CWindowsEngine::PostInit()
{
    renderingEngine->PostInit();

    for (auto& obj : gObjects)
    {
        obj->BeginInit();
    }
	return 0;
}

void CWindowsEngine::Tick(float deltaTime)
{
    for (auto& obj : gObjects)
    {
        if(obj->IsTick())
            obj->Tick(deltaTime);
    }

    if (world)
    {
        if (CCamera* camera = world->GetCamera())
        {
            FViewportInfo viewportInfo;
            viewportInfo.viewMatrix = camera->viewMatrix;
            viewportInfo.projectMatrix = camera->projectMatrix;
            renderingEngine->UpdateCalculations(deltaTime, viewportInfo);
        }
    }

    renderingEngine->Tick(deltaTime);
}

int CWindowsEngine::PreExit()
{
    renderingEngine->PreExit();
    Engine_Log("Engine pre exit complete.");
    return 0;
}

int CWindowsEngine::Exit()
{
    renderingEngine->Exit();
    Engine_Log("Engine exit complete.");
    return 0;
}

int CWindowsEngine::PostExit()
{
    FEngineRenderConfig::Destroy();
    renderingEngine->PostExit();
    Engine_Log("Engine post exit complete.");
    return 0;
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
    return true;
}
#endif