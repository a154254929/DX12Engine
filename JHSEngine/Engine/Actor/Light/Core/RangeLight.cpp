#include "RangeLight.h"

#include "../../../Component/Light/Core/RangeLightComponent.h"

GRangeLight::GRangeLight()
    : Super()
{
    
}

float GRangeLight::GetStartAttenuation() const
{
    if (const CRangeLightComponent* rangeLight = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
    {
        return rangeLight->GetStartAttenuation();
    }
    return 0.f;
}

float GRangeLight::GetEndAttenuation() const
{
    if (const CRangeLightComponent* rangeLight = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
    {
        return rangeLight->GetEndAttenuation();
    }
    return 0.f;
}

void GRangeLight::SetStartAttenuation(const float inStartAttenuation)
{
    if (CRangeLightComponent* rangeLight = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
    {
        rangeLight->SetStartAttenuation(inStartAttenuation);
    }
}

void GRangeLight::SetEndAttenuation(const float inEndAttenuation)
{
    if (CRangeLightComponent* rangeLight = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
    {
        rangeLight->SetEndAttenuation(inEndAttenuation);
    }
}