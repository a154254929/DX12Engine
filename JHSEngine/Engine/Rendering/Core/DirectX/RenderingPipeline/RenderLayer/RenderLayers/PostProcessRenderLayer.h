#pragma once
#include "../Core/RenderLayer.h"

class FPostProcessRenderLayer : public FRenderLayer
{
public:
    FPostProcessRenderLayer();

    virtual void BuildShader();
    
    virtual int FetRenderLayerType() const {retur 3;}
};
