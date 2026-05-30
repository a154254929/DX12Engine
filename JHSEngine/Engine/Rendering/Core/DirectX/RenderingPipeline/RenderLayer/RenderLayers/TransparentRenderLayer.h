#pragma once
#include "../Core/RenderLayer.h"

class FTransparentRenderLayer : public FRenderLayer
{
public:
    FTransparentRenderLayer();

    virtual void BuildShader();
    
    virtual int FetRenderLayerType() const {return 2;}
};