#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
    FLight();
    XMFLOAT3 lightIntensity;
    float startAttenuation;
    
    XMFLOAT3 lightDirection;
    float endAttenuation;
    
    XMFLOAT3 lightPosition;
    int lightType;
    
    float conicalInnerCorner; // Radian
    float conicalOuterCorner; // Radian
    XMFLOAT2 xx;
};

struct FLightConstantBuffer
{
    FLightConstantBuffer();
    
    XMINT4 lightInfo;
    FLight sceneLights[16];
};