#pragma once
#include "../Core/Engine.h"

class FViewportInfo
{
public:
	//定义摄像机位置,UP, Right, Look
	XMFLOAT4X4 viewMatrix;
	//远近裁剪面 FOV
	XMFLOAT4X4 projectMatrix;
};