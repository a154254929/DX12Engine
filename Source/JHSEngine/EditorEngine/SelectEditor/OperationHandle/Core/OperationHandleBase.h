#pragma once
#include "Engine/Actor/Core/ActorObject.h"
#include "Engine/Component/Mesh/CustomMeshComponent.h"
#include "Engine/Interface/DirectXDeviceInterface.h"
#include "../../../../Engine/Component/Input/InputType.h"

class CInputComponent;
class GOperationHandleBase :
    public GActorObject,
    public IDirectXDeviceInterface
{
    typedef GActorObject Super;
    
protected:
    enum ESelectAxisType
    {
        ESelectAxis_None,
        ESelectAxis_X,
        ESelectAxis_Y,
        ESelectAxis_Z,
        ESelectAxis_Any
    };
    
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
    
    ESelectAxisType GetSelectAxisType();
    
    virtual void SetPosition(const XMFLOAT3& inPosition) override;
    virtual void SetRotation(const fvector_3d& inRotation) override;
    virtual void SetScale(const fvector_3d& inScale) override;
    
public:
    void ResetVisible();
    
    void ResetVisible(CMeshComponent* inAxisComponent, bool inVisible);
    
    void ResetColor();
    
    void ResetColor(CCustomMeshComponent* inAxisComponent, const fvector_4d& incolor);
    
public:
    virtual void BeginInit();
    virtual void Tick(float deltaTime);
    
    void SetVisible(bool inVisible);
   // virtual bool IsVisible() const;
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    virtual void OnCaptureKeyboardInformation(const FInputKey& inputKey);
    
protected:
    bool GetRayInterHitPosition(int x, int y, fvector_3d& outHitPosition);
    
    bool bOperationHandleSelect{ false };
    float fixedZoom;
};