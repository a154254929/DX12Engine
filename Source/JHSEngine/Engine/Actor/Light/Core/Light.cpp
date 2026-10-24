#include "Light.h"

GLight::GLight()
{
}

void GLight::Tick(float deltaTime)
{
    frotator rotationV3 = GetRotation();
    
    //rotationV3.x += deltaTime * 15.0f;
    //rotationV3.y += deltaTime * 25.0f;
    //rotationV3.x += deltaTime * 15.0f;
    
    SetRotation(fvector_3d(rotationV3.pitch, rotationV3.yaw, rotationV3.roll));
}

void GLight::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    lightComponent->SetPosition(inPosition);
}

void GLight::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    lightComponent->SetRotation(inRotation);
}

void GLight::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    lightComponent->SetScale(inScale);
}

void GLight::SetLightIntensity(const fvector_3d& inLightIntensity)
{
    lightComponent->SetLightIntensity(inLightIntensity);
}

void GLight::SetLightComponent(CLightComponent* inComponent)
{
    lightComponent = inComponent;
}