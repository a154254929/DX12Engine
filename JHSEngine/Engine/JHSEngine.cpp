#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

int Init(CEngine *inEngine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#ifdef _WIN32
	FWinMainCommandParameters winMainParameters(hInstance, prevInstance, cmdLine, showCmd);
#endif

	int returnValue = inEngine->PreInit(
#ifdef _WIN32
		winMainParameters
#endif
	);
	if (returnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initiialization error, check and initializion problem.", returnValue);
		return returnValue;
	}

	returnValue = inEngine->Init(
#ifdef _WIN32
		winMainParameters
#endif
	);
	if (returnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initiialization error, check and initializion problem.", returnValue);
		return returnValue;
	}

	returnValue = inEngine->PostInit();
	if (returnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initiialization error, check and initializion problem.", returnValue);
		return returnValue;
	}

	return returnValue;
}

void Tick(CEngine* inEngine)
{
	float deltaTime = 0.03f;
	inEngine->Tick(deltaTime);
	Sleep(deltaTime * 1000);
}

int Exit(CEngine *inEngine)
{
	int returnValue = inEngine->PreExit();
	if (returnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed", returnValue);
		return returnValue;
	}

	returnValue = inEngine->Exit();
	if (returnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed", returnValue);
		return returnValue;
	}

	returnValue = inEngine->PostExit();
	if (returnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed", returnValue);
		return returnValue;
	}

	return returnValue;
}

CEngine* engine = NULL;

// hInstance 自己的实例
// prevInstance 上次实例
// cmdLine 传递命令
// showCmd 以什么方式显示
	//// SW_HIDE 隐藏窗口并激活另外一个窗口
	//// SW_MINIMIZE 最小化指定的窗口，并激活在系统表中的顶层窗口
	//// SW_RESTORE 激活并显示窗口。如果窗口已经最小化或最大化，系统将以恢复到原来的尺寸和位置显示窗口(与SW_SHOWNORMAL相同)
	//// SW_SHOW 激活一个窗口并以原来的尺寸和位置显示窗口
	//// SW_SHOWMAXIMIZED 激活窗口并最大化
	//// SW_SHOWMINIMIZED 激活窗口并最小化
	//// SW_SHOWMINNOACTIVE 将一个窗口显示为图标。激活窗口维持活动状态
	//// SW_SHOWNA 以窗口的当前状态显示窗口。激活窗口保持活动状态
	//// SW_SHOWNOACTIVATE 以窗口的最近一次的尺寸和位置显示窗口。不激活窗口
	//// SW_SHOWNORMAL 激活并显示窗口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	int returnValue = 0;
	engine = FEngineFactory::CreateEngine();
	if (engine)
	{
		//初始化
		returnValue = Init(engine, hInstance, prevInstance, cmdLine, showCmd);

		MSG engineMsg = { 0 };

		//渲染出图
		while (engineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE			消息不从队列里除掉
			//PM_REMOVE				消息从队列里除掉
			//PM_MOTIFY				此标志使系统不是发等待程序空闲的线程
			//
			//PM_QS_INPUT			处理鼠标和键盘消息
			//PM_QS_PAINT			处理画图消息
			//PM_QS_POSTMESSAGE		处理所有被寄送的消息,包括计时器和热键
			//PM_QS_SENDMESSAGE		处理所有发送消息
			if (PeekMessage(&engineMsg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&engineMsg);
				DispatchMessage(&engineMsg);
			}
			else
			{
				Tick(engine);
			}
		}

		//退出
		returnValue = Exit(engine);

		returnValue = 0;
	}
	else
	{
		returnValue = 1;
	}

	Engine_Log("[%i]Engine exited", returnValue);
	return returnValue;
}