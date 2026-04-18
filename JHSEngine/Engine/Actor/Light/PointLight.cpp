#include "PointLight.h"

GPointLight::GPointLight()
{
    PointLightComponent = CreateObject<CPointLightComponent>(new CPointLightComponent());
}

void GPointLight::Tick(float deltaTime)
{
    fvector_3d rotationV3 = GetRotation();
    
    //rotationV3.x += deltaTime * 15.0f;
    //rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(rotationV3);
}

void GPointLight::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    PointLightComponent->SetPosition(inPosition);
}

void GPointLight::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    PointLightComponent->SetRotation(inRotation);
}

void GPointLight::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    PointLightComponent->SetScale(inScale);
}

void GPointLight::SetLightIntensity(const fvector_3d& inLightIntensity)
{
    PointLightComponent->SetLightIntensity(inLightIntensity);
}

void GPointLight::SetStartAttenuation(const float inStartAttenuation)
{
    PointLightComponent->SetStartAttenuation(inStartAttenuation);
}

void GPointLight::SetEndAttenuation(const float inEndAttenuation)
{
    PointLightComponent->SetEndAttenuation(inEndAttenuation);
}
