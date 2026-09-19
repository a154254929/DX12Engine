#pragma once
#include "Core/Light.h"

class GParallelLight :public GLight
{
    typedef GLight Super;
public:
    GParallelLight();

    virtual void Tick(float deltaTime);
};