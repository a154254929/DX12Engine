#pragma once
#include "Core/LightComponent.h"

class CSpotLightComponent : public CLightComponent
{
    typedef CLightComponent Super;
public:
    CSpotLightComponent();
    
    const float GetStartAttenuation(){return startAttenuation;}
    void SetStartAttenuation(float inStartAttenuation){startAttenuation = inStartAttenuation;}
    const float GetEndAttenuation(){return endAttenuation;}
    void SetEndAttenuation(float inEndAttenuation){endAttenuation = inEndAttenuation;}
    
protected:
    float startAttenuation;
    float endAttenuation;
};