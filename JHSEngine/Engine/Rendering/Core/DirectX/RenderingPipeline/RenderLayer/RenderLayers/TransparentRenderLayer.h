#pragma once
#include "../Core/RenderLayer.h"

class FTransparentRenderLayer : public FRenderLayer
{
public:
    FTransparentRenderLayer();

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 2;}
    
    virtual void BuildPSO();
};