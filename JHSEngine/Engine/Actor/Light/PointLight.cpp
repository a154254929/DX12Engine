#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"

GPointLight::GPointLight()
    : Super()
{
    SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

void GPointLight::Tick(float deltaTime)
{
    time += deltaTime;
    XMFLOAT3 POSItion = GetPosition();
    
    POSItion.x = sin(time) * 3.0f;
    POSItion.y = 1.0f;
    POSItion.z = cos(time) * 3.0f;
    
    SetPosition(POSItion);
}