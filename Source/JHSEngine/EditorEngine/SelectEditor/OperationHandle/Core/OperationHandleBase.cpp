#include "OperationHandleBase.h"

#include "Engine/EngineType.h"
#include "Engine/Component/InputComponent.h"
#include "Engine/Library/RaycastSystemLibrary.h"
#include "Engine/Mesh/Core/Material/Material.h"

extern CMeshComponent* selectedAxisComponent;

GOperationHandleBase::GOperationHandleBase()
{
    FCreateObjectParam param;
    param.owner = this;
    inputComponent = ConstructionObject<CInputComponent>(param);
}

void GOperationHandleBase::SetMeshRenderLayerType(EMeshRenderLayerType inRenderLayerType)
{
    xAxisComponent->SetRenderLayerType(inRenderLayerType);
    yAxisComponent->SetRenderLayerType(inRenderLayerType);
    zAxisComponent->SetRenderLayerType(inRenderLayerType);
}

GOperationHandleBase::ESelectAxisType GOperationHandleBase::GetSelectAxisType()
{
    if (selectedAxisComponent == xAxisComponent)
    {
        return ESelectAxis_X;
    }
    else if (selectedAxisComponent == yAxisComponent)
    {
        return ESelectAxis_Y;
    }
    else if (selectedAxisComponent == zAxisComponent)
    {
        return ESelectAxis_Z;
    }
    return ESelectAxis_None;
}

void GOperationHandleBase::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    if (xAxisComponent)
    {
        xAxisComponent->SetPosition(inPosition);
    }
    if (yAxisComponent)
    {
        yAxisComponent->SetPosition(inPosition);
    }
    if (zAxisComponent)
    {
        zAxisComponent->SetPosition(inPosition);
    }
}

void GOperationHandleBase::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    if (xAxisComponent)
    {
        xAxisComponent->SetRotation(inRotation);
    }
    if (yAxisComponent)
    {
        yAxisComponent->SetRotation(inRotation);
    }
    if (zAxisComponent)
    {
        zAxisComponent->SetRotation(inRotation);
    }
}

void GOperationHandleBase::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    if (xAxisComponent)
    {
        xAxisComponent->SetScale(inScale);
    }
    if (yAxisComponent)
    {
        yAxisComponent->SetScale(inScale);
    }
    if (zAxisComponent)
    {
        zAxisComponent->SetScale(inScale);
    }
}

void GOperationHandleBase::ResetColor()
{
    ResetColor(xAxisComponent, fvector_4d(1.f, 0.f, 0.f, 1.f));
    ResetColor(yAxisComponent, fvector_4d(0.f, 1.f, 0.f, 1.f));
    ResetColor(zAxisComponent, fvector_4d(0.f, 0.f, 1.f, 1.f));
}

void GOperationHandleBase::ResetColor(CCustomMeshComponent* inAxisComponent, const fvector_4d& incolor)
{
    if (inAxisComponent)
    {
        if (CMaterial* material = (*inAxisComponent->GetMaterials())[0])
        {
            material->SetBaseColor(incolor);
        }
    }
}

void GOperationHandleBase::BeginInit()
{
    Super::BeginInit();
    
    inputComponent->OnLeftMouseButtonDownDelegate.Bind(this, &GOperationHandleBase::OnLeftButtonDown);
    inputComponent->OnLeftMouseButtonUpDelegate.Bind(this, &GOperationHandleBase::OnLeftButtonUp);
    inputComponent->OnMouseMoveDelegate.Bind(this, &GOperationHandleBase::OnMouseMove);
}

void GOperationHandleBase::OnMouseMove(int x, int y)
{
    if (bOperationHandleSelect)
    {
        return;
    }
    FCollisionResult collisionResult;
    FRaycastSystemLibrary::HitSpecialObjectsResultByScreen(GetWorld(), this, x, y, collisionResult);
    
    ResetColor();
    if (collisionResult.bIsCollided)
    {
        CCustomMeshComponent* selectCustomMeshComponent = dynamic_cast<CCustomMeshComponent*>(collisionResult.collisionComponent);
        ResetColor(selectCustomMeshComponent, fvector_4d(1.f, 1.f, 0.f, 1.f));
        selectedAxisComponent = selectCustomMeshComponent;
    }
    else
    {
        selectedAxisComponent = nullptr;
    }
}

void GOperationHandleBase::OnLeftButtonDown(int x, int y)
{
    bOperationHandleSelect = true;
}

void GOperationHandleBase::OnLeftButtonUp(int x, int y)
{
    bOperationHandleSelect = false;
    if (selectedAxisComponent)
    {
        ResetColor();
        selectedAxisComponent = nullptr;
    }
}
