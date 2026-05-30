#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueRenderLayer : public FRenderLayer
{
public:
    FOpaqueRenderLayer();
    

    virtual void BuildShader();
};