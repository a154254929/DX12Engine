#include "SpotLight.h"

GSpotLight::GSpotLight()
{
    SpotLightComponent = CreateObject<CSpotLightComponent>(new CSpotLightComponent());
}

void GSpotLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    rotationV3.x += deltaTime * 15.0f;
    //rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(rotationV3);
}

void GSpotLight::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    SpotLightComponent->SetPosition(inPosition);
}

void GSpotLight::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    SpotLightComponent->SetRotation(inRotation);
}

void GSpotLight::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    SpotLightComponent->SetScale(inScale);
}

void GSpotLight::SetLightIntensity(const fvector_3d& inLightIntensity)
{
    SpotLightComponent->SetLightIntensity(inLightIntensity);
}

void GSpotLight::SetStartAttenuation(const float inStartAttenuation)
{
    SpotLightComponent->SetStartAttenuation(inStartAttenuation);
}

void GSpotLight::SetEndAttenuation(const float inEndAttenuation)
{
    SpotLightComponent->SetEndAttenuation(inEndAttenuation);
}
