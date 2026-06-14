#include "Fog.h"

#include "../../Component/Sky/FogComponent.h"

GFog::GFog()
{
    fogComponent = CreateObject<CFogComponent>(new CFogComponent());
}

void GFog::SetFogColor(const fvector_color& inColor)
{
    fogComponent->SetFogColor(inColor);
}

void GFog::SetFogStart(float inStart)
{
    fogComponent->SetFogStart(inStart);
}

void GFog::SetFogRange(float inRange)
{
    fogComponent->SetFogRange(inRange);
}
