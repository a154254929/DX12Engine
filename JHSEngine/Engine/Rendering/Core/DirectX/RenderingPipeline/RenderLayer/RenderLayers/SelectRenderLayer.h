#pragma once
#include "../Core/RenderLayer.h"

class FSelectRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FSelectRenderLayer();
    
    virtual void Draw(float deltaTime);
    
    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 9;}
    
    virtual void BuildPSO();
    
    virtual void ResetPSO();
};