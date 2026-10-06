#pragma once
#include "../../Core/Engine.h"
#include "../../Math/EngineMath.h"

struct FViewportTransformation
{
    FViewportTransformation();

    XMFLOAT4 viewportWorldPosition;
    XMFLOAT4X4 viewProjectionMatrix;
};