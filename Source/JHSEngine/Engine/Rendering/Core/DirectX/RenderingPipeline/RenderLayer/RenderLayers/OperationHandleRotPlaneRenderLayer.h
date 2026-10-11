#pragma once
#include "../Core/RenderLayer.h"
#include "Engine/Component/Mesh/Core/MeshComponentType.h"

class FOperationHandleRotPlaneRenderLayer : public FRenderLayer
{
    typedef FRenderLayer Super;
public:
    FOperationHandleRotPlaneRenderLayer();
    
    virtual void Draw(float deltaTime);

    virtual void BuildShader();
    
    virtual int GetRenderLayerType() const {return RENDERLAYER_OPERATION_HANDLE_ROT_PLANE;}
    
    virtual void BuildPSO();

    virtual void ResetPSO();
};
