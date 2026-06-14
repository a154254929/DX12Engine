#pragma once
#include "../../../EngineMinimal.h"

struct FFogConstantBuffer
{
    FFogConstantBuffer();
    
    XMFLOAT3 fogColor;
    float fogStart;
    
    float fogRange;
    float fogHeight;
    float fogTransparentCoefficient;
    float XX2;
};