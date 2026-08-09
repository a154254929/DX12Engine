#pragma once
#include "../Core/DynamicMap.h"

class GClientViewPort;

class FDynamicShadowCubeMap : public FDynamicMap
{
    typedef FDynamicMap Super;
public:
    FDynamicShadowCubeMap();
    
protected:
    std::vector<GClientViewPort*> cubemapViewPorts;
};
