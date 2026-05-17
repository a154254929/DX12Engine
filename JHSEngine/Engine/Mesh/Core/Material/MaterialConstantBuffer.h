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
    
    int specularMapIndex;
    int roughnessMapIndex;
    int reversedMapIndex2;
    int reversedMapIndex3;

    XMFLOAT4 baseColor;
    XMFLOAT3 specularColor;
    int XX;
    
    XMFLOAT4X4 transformation;
};