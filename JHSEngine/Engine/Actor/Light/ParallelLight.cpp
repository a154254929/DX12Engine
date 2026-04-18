#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
    parallelLightComponent = CreateObject<CParallelLightComponent>(new CParallelLightComponent());
}

void GParallelLight::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    parallelLightComponent->SetPosition(inPosition);
}

void GParallelLight::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    parallelLightComponent->SetRotation(inRotation);
}

void GParallelLight::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    parallelLightComponent->SetScale(inScale);
}
