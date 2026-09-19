#include "Fog.h"

#include "../../Component/Sky/FogComponent.h"

GFog::GFog()
{
    FCreateObjectParam param;
    param.owner = this;
    fogComponent = CreateObject<CFogComponent>(param, new CFogComponent());
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

void GFog::SetFogHeight(float inHeight)
{
    fogComponent->SetFogHeight(inHeight);
}

void GFog::SetFogTransparentCoefficient(float inTransparentCoefficient)
{
    fogComponent->SetFogTransparentCoefficient(inTransparentCoefficient);
}
