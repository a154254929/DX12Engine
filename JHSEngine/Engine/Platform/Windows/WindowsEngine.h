#pragma once

#ifdef _WIN32
#include "../../Core/Engine.h"
#include "../../Rendering/Engine/Core/RenderingEngine.h"

class CDirectXRenderingEngine;
class CWindowsEngine : public CEngine
{
	friend class IDirectXDeviceInterface;
public:
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters inParameters);
	virtual int Init(FWinMainCommandParameters inParameters);
	virtual int PostInit();

	virtual void Tick(float deltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

    CDirectXRenderingEngine* GetRenderingEngine() { return renderingEngine; }

private:
    bool InitWindows(FWinMainCommandParameters InParameters);

protected:
	HWND mainWindowsHandle;         //主窗口句柄

protected:
    CDirectXRenderingEngine* renderingEngine;
};
#endif