#pragma once
#include "../Core/RenderLayer.h"

class FBackGroundRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FBackGroundRenderLayer();
    
    virtual void Draw(float deltaTime);
    
    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 4;}
    
    virtual void BuildPSO();
};