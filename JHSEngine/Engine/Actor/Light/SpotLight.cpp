#include "SpotLight.h"
#include "../../Component/Light/SpotLightComponent.h"

GSpotLight::GSpotLight()
    : Super()
{
    SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
}

void GSpotLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    rotationV3.x += deltaTime * 15.0f;
    //rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(rotationV3);
}
