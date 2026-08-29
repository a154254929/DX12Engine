#include "ParallelLight.h"
#include "../../Component/Light/ParallelLightComponent.h"

GParallelLight::GParallelLight()
    : Super()
{
    FCreateObjectParam param;
    param.owner = this;
    SetLightComponent(CreateObject<CParallelLightComponent>(param, new CParallelLightComponent()));
}

void GParallelLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    //rotationV3.x += deltaTime * 15.0f;
    rotationV3.y += deltaTime * 15.0f;
    //rotationV3.x += deltaTime * 10.0f;
    
    SetRotation(rotationV3);
}
