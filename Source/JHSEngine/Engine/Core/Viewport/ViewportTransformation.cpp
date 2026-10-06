#include "ViewportTransformation.h"

FViewportTransformation::FViewportTransformation()
    : viewProjectionMatrix(EngineMath::IdentityMatrix4x4())
    , viewportWorldPosition(0.f, 0.f, 0.f, 1.f)
{
}