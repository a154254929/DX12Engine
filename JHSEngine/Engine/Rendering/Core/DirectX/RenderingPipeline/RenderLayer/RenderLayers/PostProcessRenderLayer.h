#pragma once
#include "../Core/RenderLayer.h"

class FPostProcessRenderLayer : public FRenderLayer
{
public:
    FPostProcessRenderLayer();

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 3;}
};
