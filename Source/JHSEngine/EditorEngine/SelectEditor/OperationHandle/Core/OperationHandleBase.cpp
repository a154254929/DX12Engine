#include "OperationHandleBase.h"

#include "Engine/EngineType.h"
#include "Engine/Component/InputComponent.h"
#include "Engine/Library/RaycastSystemLibrary.h"
#include "Engine/Mesh/Core/Material/Material.h"

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
    FCollisionResult collisionResult;
    FRaycastSystemLibrary::HitSpecialObjectsResultByScreen(GetWorld(), this, x, y, collisionResult);
    
    ResetColor();
    if (collisionResult.bIsCollided)
    {
        CCustomMeshComponent* selectCustomMeshComponent = dynamic_cast<CCustomMeshComponent*>(collisionResult.collisionComponent);
        ResetColor(selectCustomMeshComponent, fvector_4d(1.f, 1.f, 0.f, 1.f));
    }
}

void GOperationHandleBase::OnLeftButtonDown(int x, int y)
{
}

void GOperationHandleBase::OnLeftButtonUp(int x, int y)
{
}
