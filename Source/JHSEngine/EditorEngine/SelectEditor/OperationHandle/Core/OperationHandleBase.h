#pragma once
#include "Engine/Actor/Core/ActorObject.h"
#include "Engine/Component/Mesh/CustomMeshComponent.h"
#include "Engine/Interface/DirectXDeviceInterface.h"

class GOperationHandleBase :
    public GActorObject,
    public IDirectXDeviceInterface
{
    typedef GActorObject Super;
    
protected:
    CVARIABLE()
    CCustomMeshComponent* xAxisComponent;
    
    CVARIABLE()
    CCustomMeshComponent* yAxisComponent;
    
    CVARIABLE()
    CCustomMeshComponent* zAxisComponent;
    
    
public:
    GOperationHandleBase();
    
    virtual void SetMeshRenderLayerType(EMeshRenderLayerType inRenderLayerType);
};