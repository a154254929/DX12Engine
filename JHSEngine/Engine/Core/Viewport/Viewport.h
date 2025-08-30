#pragma once
#include "../Engine.h"

class FViewport
{
public:
	FViewport();

	void ViewportInit();

	//定义摄像机位置,UP, Right, Look
	XMFLOAT4X4 viewMatrix;
	//远近裁剪面 FOV
	XMFLOAT4X4 projectMatrix;
};