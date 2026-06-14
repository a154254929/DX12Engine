#include "FogComponent.h"

CFogComponent::CFogComponent()
    : fogColor (1.0f, 1.0f,1.0f,1.0f)
    , fogStart(5.0f)
    , fogRange(100.0f)
    , fogHeight(100.0f)
    , bDirty(false)
{
}

void CFogComponent::SetFogColor(const fvector_color& inColor)
{
    fogColor = inColor;
    SetDirty(true);
}

void CFogComponent::SetFogStart(float inStart)
{
    fogStart = inStart;
    SetDirty(true);
}

void CFogComponent::SetFogRange(float inRange)
{
    fogRange = inRange;
    SetDirty(true);
}

void CFogComponent::SetFogHeight(float inHeight)
{
    fogHeight = inHeight;
    SetDirty(true);
}

void CFogComponent::SetDirty(bool inDirty)
{
    bDirty = true;
}
