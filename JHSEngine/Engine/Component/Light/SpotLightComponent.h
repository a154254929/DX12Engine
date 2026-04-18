#pragma once
#include "Core/RangeLightComponent.h"

class CSpotLightComponent : public CRangeLightComponent
{
    typedef CRangeLightComponent Super;
public:
    CSpotLightComponent();
    
public:
    void SetConicalInnerCorner(float inConicalInnerCorner);
    void SetConicalOuterCorner(float inConicalOuterCorner);
    
public:
    float GetConicalInnerCorner() const {return conicalInnerCorner;}
    float GetConicalOuterCorner() const {return conicalOuterCorner;}
protected:
    float conicalInnerCorner; // Angle
    float conicalOuterCorner; // Angle
};