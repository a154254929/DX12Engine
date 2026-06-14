#pragma once
#include "../../../EngineMinimal.h"

struct FFog
{
    FFog();
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

struct FFogConstantBuffer
{
    FFogConstantBuffer();
    
    XMFLOAT3 fogColor;
    float fogStart;
    
    float fogRange;
    float fogHeight;
};