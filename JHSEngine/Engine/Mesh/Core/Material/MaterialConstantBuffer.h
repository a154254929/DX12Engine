#pragma once
#include "../../../Math/EngineMath.h"

class FMaterialConstantBuffer
{
public:
    FMaterialConstantBuffer();

    UINT32 materialType;
    float roughness;
    int baseColorIndex;
    UINT32 xx3;

    XMFLOAT4 baseColor;
    XMFLOAT4X4 transformation;
};