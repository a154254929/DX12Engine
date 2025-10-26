#pragma once
#include "../../../EngineMinimal.h"

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 lightIntensity;
	XMFLOAT3 lightDirection;
};