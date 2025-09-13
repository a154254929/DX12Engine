#pragma once
#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#include "../../Core/Engine.h"
#endif

//提供渲染内容的接口
class IRenderingInterface : public IGuidInterface
{
	friend class CWindowsEngine;
public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

private:
	static vector<IRenderingInterface*> renderingInterfaces;
};