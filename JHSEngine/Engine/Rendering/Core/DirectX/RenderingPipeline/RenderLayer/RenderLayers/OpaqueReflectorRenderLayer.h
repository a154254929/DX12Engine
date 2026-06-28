#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueReflectorRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FOpaqueReflectorRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return 0;}
    
    virtual void BuildPSO();
};