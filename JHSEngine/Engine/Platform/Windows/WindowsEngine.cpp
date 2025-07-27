#include "WindowsEngine.h"
#ifdef _WIN32
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters inParameters)
{
    //日志系统初始化
    const char logPath[] = "../log";
    init_log_system(logPath);
    Engine_Log("Log Init.");
     
    //处理命令


    if (InitWindows(inParameters))
    {

    }


    Engine_Log("Engine pre initialization complete.");
    return 0;
}

int FWindowsEngine::Init()
{
    Engine_Log("Engine initialization complete.");
    return 0;
}

int FWindowsEngine::PostInit()
{
    Engine_Log("Engine post initialization complete.");
    return 0;
}

void FWindowsEngine::Tick()
{
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
    Engine_Log("Engine post exit complete.");
    return 0;
}
bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
    //注册窗口
    WNDCLASSEX windowsClass;
    windowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
    windowsClass.cbClsExtra = 0;//是否需要额外空间
    windowsClass.cbWndExtra = 0;//是否需要额外内存
    windowsClass.hbrBackground = nullptr;//是否设置GDI擦除
    windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//设置箭头光标
    windowsClass.hIcon = nullptr;//应用程序在磁盘上显示的图标
    windowsClass.hIconSm = nullptr;//应用程序显示在左上角的图标
    windowsClass.hInstance = InParameters.HInstance;//窗口实例
    windowsClass.lpszClassName = L"JHSEngine";//窗口实例
    windowsClass.lpszMenuName = nullptr;//菜单名称
    windowsClass.style = CS_VREDRAW | CS_HREDRAW;//怎么绘制窗口 垂直和水平重绘
    windowsClass.lpfnWndProc = EngineWindowProc;//消息处理函数

    //注册窗口
    ATOM registerAtom = RegisterClassEx(&windowsClass);

    if (!registerAtom)
    {
        Engine_Log_Error("Register Fail.");
        MessageBox(NULL, L"Register windows class fail", L"Error", MB_OK);
    }

    RECT rect = { 0, 0, 1280, 720 };

    //lpRect 视口
    //dwStyle 视口风格
    //bMenu 菜单
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindowEx(
        NULL,                       //窗口额外风格
        L"JHSEngine",               //窗口名称
        L"JHS Engine",              //标题栏名称
        WS_OVERLAPPEDWINDOW,        //窗口风格
        100, 100,                   //窗口坐标
        windowWidth, windowHeight,  //窗口宽高
        NULL,                       //副窗口句柄
        nullptr,                    //菜单句柄
        InParameters.HInstance,     //窗口实例
        NULL                        //额外参数
    );

    if (!hwnd)
    {
        Engine_Log_Error("InitWindow fail.");
        MessageBox(0, L"CreateWindow Failed", 0, 0);
        return false;
    }

    //显示窗口
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    Engine_Log("InitWindow complete.");
}
#endif
