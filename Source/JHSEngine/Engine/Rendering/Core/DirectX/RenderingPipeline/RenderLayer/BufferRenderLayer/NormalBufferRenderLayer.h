#pragma once
#include "../Core/RenderLayer.h"

class FNormalBufferRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FNormalBufferRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 20;}
    
    virtual void BuildPSO();

    virtual void ResetPSO();
};