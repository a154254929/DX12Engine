#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueShadowRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FOpaqueShadowRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 8;}
    
    virtual void BuildPSO();

    virtual void ResetPSO();
public:  
};
