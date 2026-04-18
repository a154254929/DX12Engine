#include "ParallelLight.h"
#include "../../Component/Light/ParallelLightComponent.h"

GParallelLight::GParallelLight()
    : Super()
{
    SetLightComponent(CreateObject<CParallelLightComponent>(new CParallelLightComponent()));
}

void GParallelLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    //rotationV3.x += deltaTime * 15.0f;
    //rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(rotationV3);
}
