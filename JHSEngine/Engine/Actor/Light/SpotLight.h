#pragma once
#include "Core/RangeLight.h"

class GSpotLight :public GRangeLight
{
    typedef GRangeLight Super;
public:
    GSpotLight();

    virtual void Tick(float deltaTime);
public:
    void SetConicalInnerCorner(const float inConicalInnerCorner);
    void SetConicalOuterCorner(const float inConicalOuterCorner);
    float GetConicalInnerCorner() const;
    float GetConicalOuterCorner() const;
};