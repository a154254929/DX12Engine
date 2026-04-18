#pragma once
#include "LightComponent.h"

class CRangeLightComponent : public CLightComponent
{
    typedef CLightComponent Super;
public:
    CRangeLightComponent();
    
    float GetStartAttenuation() const {return startAttenuation;}
    void SetStartAttenuation(float inStartAttenuation){startAttenuation = inStartAttenuation;}
    float GetEndAttenuation() const {return endAttenuation;}
    void SetEndAttenuation(float inEndAttenuation){endAttenuation = inEndAttenuation;}
    
protected:
    float startAttenuation;
    float endAttenuation;
};
