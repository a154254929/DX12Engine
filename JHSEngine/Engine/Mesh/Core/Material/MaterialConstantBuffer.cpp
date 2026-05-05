#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
    : baseColor(.5f, .5f, .5f, 1.0f)
    , roughness(.2f)
    , baseColorIndex(-1)
    , transformation(EngineMath::IdentityMatrix4x4())
{

}