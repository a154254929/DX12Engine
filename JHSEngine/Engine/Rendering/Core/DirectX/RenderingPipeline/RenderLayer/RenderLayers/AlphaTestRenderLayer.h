#pragma once
#include "../Core/RenderLayer.h"

class FAlphaTestRenderLayer : public FRenderLayer
{
public:
    FAlphaTestRenderLayer();
    
    virtual void BuildShader();
    
    virtual int FetRenderLayerType() const {return 1;}
};