#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
    FObjectTransformation();

    XMFLOAT4X4 world;
    XMFLOAT4X4 textureTransformation;
    UINT materialIndex;
    UINT RR0;
    UINT RR1;
    UINT RR2;
};