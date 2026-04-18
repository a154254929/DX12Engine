#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
    parallelLightComponent = CreateObject<CParallelLightComponent>(new CParallelLightComponent());
}

void GParallelLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    //rotationV3.x += deltaTime * 15.0f;
    rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(rotationV3);
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
