#pragma once
#include "../../../EngineMinimal.h"

struct FFogConstantBuffer
{
    FFogConstantBuffer();
    
    XMFLOAT3 fogColor;
    float fogStart;
    
    float fogRange;
    float fogHeight;
    float XX1;
    float XX2;
};