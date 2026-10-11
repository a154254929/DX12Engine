#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
    : baseColor(.5f, .5f, .5f, 1.0f)
    , roughness(.2f)
    , baseColorIndex(-1)
    , normalMapIndex(-1)
    , specularMapIndex(-1)
    , roughnessMapIndex(-1)
    , customParam0(0.0f)
    , customParam1(0.0f)
    , customParam2(0.0f)
    , customParam3(0.0f)
    , fresnelF0(0.04f, 0.04f, 0.04f)
    , transformation(EngineMath::IdentityMatrix4x4())
{

}