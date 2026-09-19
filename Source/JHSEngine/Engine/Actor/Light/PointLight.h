#pragma once
#include "Core/RangeLight.h"

class GPointLight :public GRangeLight
{
    typedef GRangeLight Super;
public:
    GPointLight();

    virtual void Tick(float deltaTime);
    
private:
    float time = 0.f;
};