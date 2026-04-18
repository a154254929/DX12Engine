#pragma once
#include "Core/RangeLight.h"

class GPointLight :public GRangeLight
{
    typedef GRangeLight Super;
public:
    GPointLight();

    virtual void Tick(float deltaTime);
};