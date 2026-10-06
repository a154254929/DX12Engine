#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
    FObjectTransformation();

    XMFLOAT4X4 world;
    XMFLOAT4X4 textureTransformation;
    XMFLOAT4X4 normalWorldMatrix;
    UINT materialIndex;
    UINT RR0;
    UINT RR1;
    UINT RR2;
};