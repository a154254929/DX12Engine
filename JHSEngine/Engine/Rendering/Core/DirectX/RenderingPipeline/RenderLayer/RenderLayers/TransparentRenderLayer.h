#pragma once
#include "../Core/RenderLayer.h"

class FTransparentRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FTransparentRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 2;}
    
    virtual void BuildPSO();
};