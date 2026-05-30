#pragma once
#include "../Core/RenderLayer.h"

class FPostProcessRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FPostProcessRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 3;}
    
    virtual void BuildPSO();
};
