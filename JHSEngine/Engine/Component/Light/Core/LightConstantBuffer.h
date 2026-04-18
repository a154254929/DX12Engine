#pragma once
#include "../../../EngineMinimal.h"

struct FLightConstantBuffer
{
    FLightConstantBuffer();

    XMFLOAT3 lightIntensity;
    float XX = 0.f;
    XMFLOAT3 lightDirection;
    float xx1 = 0.f;
};