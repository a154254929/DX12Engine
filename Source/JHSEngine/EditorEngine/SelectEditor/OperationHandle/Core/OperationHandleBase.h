#pragma once
#include "Engine/Actor/Core/ActorObject.h"
#include "Engine/Component/Mesh/CustomMeshComponent.h"
#include "Engine/Interface/DirectXDeviceInterface.h"

class CInputComponent;
class GOperationHandleBase :
    public GActorObject,
    public IDirectXDeviceInterface
{
    typedef GActorObject Super;
    
protected:
    CVARIABLE()
    CInputComponent* inputComponent;
    
    CVARIABLE()
    CCustomMeshComponent* xAxisComponent;
    
    CVARIABLE()
    CCustomMeshComponent* yAxisComponent;
    
    CVARIABLE()
    CCustomMeshComponent* zAxisComponent;
    
    
public:
    GOperationHandleBase();
    
    virtual void SetMeshRenderLayerType(EMeshRenderLayerType inRenderLayerType);
    
public:
    void ResetColor();
    
    void ResetColor(CCustomMeshComponent* inAxisComponent, const fvector_4d& incolor);
    
public:
    virtual void BeginInit();
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
};