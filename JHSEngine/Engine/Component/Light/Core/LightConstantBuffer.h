#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
    XMFLOAT3 lightIntensity;
    float XX = 0.f;
    XMFLOAT3 lightDirection;
    float xx1 = 0.f;
};

struct FLightConstantBuffer
{
    FLightConstantBuffer();
    
    XMINT4 lightInfo;
    FLight screenLight[16];
};