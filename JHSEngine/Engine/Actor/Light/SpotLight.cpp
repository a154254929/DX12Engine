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

void GSpotLight::SetConicalInnerCorner(float inConicalInnerCorner)
{
    if (CSpotLightComponent* spotLight = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
    {
        spotLight->SetConicalInnerCorner(inConicalInnerCorner);
    }
}

void GSpotLight::SetConicalOuterCorner(const float inConicalOuterCorner)
{
    if (CSpotLightComponent* spotLight = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
    {
        spotLight->SetConicalOuterCorner(inConicalOuterCorner);
    }
}

float GSpotLight::GetConicalInnerCorner() const
{
    if (const CSpotLightComponent* spotLight = dynamic_cast<const CSpotLightComponent*>(GetLightComponent()))
    {
        return spotLight->GetConicalInnerCorner();
    }
    return 0.f;
}

float GSpotLight::GetConicalOuterCorner() const
{
    if (const CSpotLightComponent* spotLight = dynamic_cast<const CSpotLightComponent*>(GetLightComponent()))
    {
        return spotLight->GetConicalInnerCorner();
    }
    return 0.f;
}
