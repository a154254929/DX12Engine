#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
    : baseColor(.5f, .5f, .5f, 1.0f)
    , roughness(.2f)
    , baseColorIndex(-1)
    , normalMapIndex(-1)
    , specularMapIndex(-1)
    , roughnessMapIndex(-1)
    , reversedMapIndex2(-1)
    , reversedMapIndex3(-1)
    , fresnelF0(0.04f, 0.04f, 0.04f)
    , transformation(EngineMath::IdentityMatrix4x4())
{

}