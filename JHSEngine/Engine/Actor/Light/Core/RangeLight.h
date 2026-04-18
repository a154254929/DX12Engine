#pragma once
#include "Light.h"

class GRangeLight :public GLight
{
    typedef GLight Super;
public:
    GRangeLight();
    
public:
    void SetStartAttenuation(const float inStartAttenuation);
    void SetEndAttenuation(const float inEndAttenuation);
public:
    float GetStartAttenuation() const;
    float GetEndAttenuation() const;
};