#pragma once
#include "../../../Math/EngineMath.h"

class FMaterialConstantBuffer
{
public:
    FMaterialConstantBuffer();

    UINT32 materialType;
    float roughness;
    int baseColorIndex;
    int normalMapIndex;

    XMFLOAT4 baseColor;
    XMFLOAT4X4 transformation;
};