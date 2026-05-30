#pragma once
#include "../Core/RenderLayer.h"

class FAlphaTestRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FAlphaTestRenderLayer();
    
    virtual void Draw(float deltaTime);
    
    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 1;}
    
    virtual void BuildPSO();
};