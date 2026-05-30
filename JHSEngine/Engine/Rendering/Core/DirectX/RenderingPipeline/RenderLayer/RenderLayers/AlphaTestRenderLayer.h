#pragma once
#include "../Core/RenderLayer.h"

class FAlphaTestRenderLayer : public FRenderLayer
{
public:
    FAlphaTestRenderLayer();
    
    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 1;}
};