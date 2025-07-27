#include "WindowsEngine.h"
#ifdef _WIN32
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters inParameters)
{
    //��־ϵͳ��ʼ��
    const char logPath[] = "../log";
    init_log_system(logPath);
    Engine_Log("Log Init.");
     
    //��������


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
    //ע�ᴰ��
    WNDCLASSEX windowsClass;
    windowsClass.cbSize = sizeof(WNDCLASSEX);//�ö���ʵ��ռ�ö���ڴ�
    windowsClass.cbClsExtra = 0;//�Ƿ���Ҫ����ռ�
    windowsClass.cbWndExtra = 0;//�Ƿ���Ҫ�����ڴ�
    windowsClass.hbrBackground = nullptr;//�Ƿ�����GDI����
    windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//���ü�ͷ���
    windowsClass.hIcon = nullptr;//Ӧ�ó����ڴ�������ʾ��ͼ��
    windowsClass.hIconSm = nullptr;//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
    windowsClass.hInstance = InParameters.HInstance;//����ʵ��
    windowsClass.lpszClassName = L"JHSEngine";//����ʵ��
    windowsClass.lpszMenuName = nullptr;//�˵�����
    windowsClass.style = CS_VREDRAW | CS_HREDRAW;//��ô���ƴ��� ��ֱ��ˮƽ�ػ�
    windowsClass.lpfnWndProc = EngineWindowProc;//��Ϣ������

    //ע�ᴰ��
    ATOM registerAtom = RegisterClassEx(&windowsClass);

    if (!registerAtom)
    {
        Engine_Log_Error("Register Fail.");
        MessageBox(NULL, L"Register windows class fail", L"Error", MB_OK);
    }

    RECT rect = { 0, 0, 1280, 720 };

    //lpRect �ӿ�
    //dwStyle �ӿڷ��
    //bMenu �˵�
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindowEx(
        NULL,                       //���ڶ�����
        L"JHSEngine",               //��������
        L"JHS Engine",              //����������
        WS_OVERLAPPEDWINDOW,        //���ڷ��
        100, 100,                   //��������
        windowWidth, windowHeight,  //���ڿ��
        NULL,                       //�����ھ��
        nullptr,                    //�˵����
        InParameters.HInstance,     //����ʵ��
        NULL                        //�������
    );

    if (!hwnd)
    {
        Engine_Log_Error("InitWindow fail.");
        MessageBox(0, L"CreateWindow Failed", 0, 0);
        return false;
    }

    //��ʾ����
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    Engine_Log("InitWindow complete.");
}
#endif
