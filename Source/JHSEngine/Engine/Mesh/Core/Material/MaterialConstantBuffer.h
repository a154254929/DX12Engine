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
    float customParam0;
    float customParam1;

    XMFLOAT4 baseColor;
    
    XMFLOAT3 specularColor;
    float customParam2;
    
    XMFLOAT3 fresnelF0;
    float customParam3;
    
    XMFLOAT4X4 transformation;
};