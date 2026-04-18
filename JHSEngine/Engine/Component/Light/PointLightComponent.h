#pragma once
#include "Core/LightComponent.h"

class CPointLightComponent : public CLightComponent
{
    typedef CLightComponent Super;
public:
    CPointLightComponent();
    
    const float GetStartAttenuation(){return startAttenuation;}
    float SetStartAttenuation(float inStartAttenuation){startAttenuation = inStartAttenuation;}
    const float GetEndAttenuation(){return endAttenuation;}
    float SetEndAttenuation(float inEndAttenuation){endAttenuation = inEndAttenuation;}
    
protected:
    float startAttenuation;
    float endAttenuation;
};