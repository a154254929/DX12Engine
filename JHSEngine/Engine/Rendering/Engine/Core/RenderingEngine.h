#pragma once
#include "../../../Core/Engine.h"

class CRenderingEngine : public CEngine
{
public:
	void SetMainWindowsHandle(HWND inNewMainWindowsHandle);

protected:
	HWND mainWindowsHandle;         //主窗口句柄
};