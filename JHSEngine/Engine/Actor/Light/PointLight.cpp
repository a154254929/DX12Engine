#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"

GPointLight::GPointLight()
    : Super()
{
    SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

void GPointLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    //rotationV3.x += deltaTime * 15.0f;
    //rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(rotationV3);
}