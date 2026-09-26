#pragma once
#include "../Core/RenderLayer.h"
#include "Engine/Component/Mesh/Core/MeshComponentType.h"

class FOperationHandleRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FOperationHandleRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return RENDERLAYER_OPERATION_HANDLE;}
    
    virtual void BuildPSO();

    virtual void ResetPSO();
};
