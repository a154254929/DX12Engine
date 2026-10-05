#include "OperationHandleBase.h"

#include "Common/OperationHandleSelectManager.h"
#include "Engine/EngineType.h"
#include "Engine/Component/InputComponent.h"
#include "Engine/Core/Camera.h"
#include "Engine/Core/World.h"
#include "Engine/Library/RaycastSystemLibrary.h"
#include "Engine/Math/EngineMath.h"
#include "Engine/Mesh/Core/Material/Material.h"

extern CMeshComponent* selectedAxisComponent;

GOperationHandleBase::GOperationHandleBase()
{
    FCreateObjectParam param;
    param.owner = this;
    inputComponent = ConstructionObject<CInputComponent>(param);
    fixedZoom = 80.f;
    
    xAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    yAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    zAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    customAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    
    SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE);
}

void GOperationHandleBase::SetMeshRenderLayerType(EMeshRenderLayerType inRenderLayerType)
{
    xAxisComponent->SetRenderLayerType(inRenderLayerType);
    yAxisComponent->SetRenderLayerType(inRenderLayerType);
    zAxisComponent->SetRenderLayerType(inRenderLayerType);
    customAxisComponent->SetRenderLayerType(inRenderLayerType);
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
    else if (selectedAxisComponent == customAxisComponent)
    {
        return ESelectAxis_Any;
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
    if (customAxisComponent)
    {
        customAxisComponent->SetPosition(inPosition);
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
    if (customAxisComponent)
    {
        customAxisComponent->SetRotation(inRotation);
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
    if (customAxisComponent)
    {
        customAxisComponent->SetScale(inScale);
    }
}

void GOperationHandleBase::ResetVisible()
{
    SetVisible(true);
}

void GOperationHandleBase::ResetVisible(CMeshComponent* inAxisComponent, bool inVisible)
{
    SetVisible(!inVisible);
    if (inAxisComponent)
    {
        inAxisComponent->SetVisible(inVisible);
    }
}

void GOperationHandleBase::ResetColor()
{
    ResetColor(xAxisComponent, fvector_4d(1.f, 0.f, 0.f, 1.f));
    ResetColor(yAxisComponent, fvector_4d(0.f, 1.f, 0.f, 1.f));
    ResetColor(zAxisComponent, fvector_4d(0.f, 0.f, 1.f, 1.f));
    ResetColor(customAxisComponent, fvector_4d(.8f, 0.2f, .0f, 1.f));
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
    
    inputComponent->captureKeyboardInforDelegate.Bind(this, &GOperationHandleBase::OnCaptureKeyboardInformation);
    inputComponent->OnLeftMouseButtonDownDelegate.Bind(this, &GOperationHandleBase::OnLeftButtonDown);
    inputComponent->OnLeftMouseButtonUpDelegate.Bind(this, &GOperationHandleBase::OnLeftButtonUp);
    inputComponent->OnMouseMoveDelegate.Bind(this, &GOperationHandleBase::OnMouseMove);
    SetVisible(false);
}

void GOperationHandleBase::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    
    if (CWorld* world = GetWorld())
    {
        if (GCamera* camera = world->GetCamera())
        {
            fvector_3d cameraPosition = EngineMath::ToVector3d(camera->GetPosition());
            fvector_3d handlePosition = EngineMath::ToVector3d(GetPosition());
            fvector_3d distanceVector = cameraPosition - handlePosition;
            float distance = distanceVector.len();
            // Keep the last valid scale when the camera is at the handle's position.
            if (distance > 1.e-4f)
            {
                SetScale(fvector_3d(distance / fixedZoom));
            }
        }
    }
}

void GOperationHandleBase::SetVisible(bool inVisible)
{
    if (xAxisComponent)
    {
        xAxisComponent->SetVisible(inVisible);
    }
    if (yAxisComponent)
    {
        yAxisComponent->SetVisible(inVisible);
    }
    if (zAxisComponent)
    {
        zAxisComponent->SetVisible(inVisible);
    }
    if (customAxisComponent)
    {
        customAxisComponent->SetVisible(inVisible);
    }
}

void GOperationHandleBase::OnMouseMove(int x, int y)
{
    if (bOperationHandleSelect || !IsCurrentOperationHandleSelect())
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
    if (!IsCurrentOperationHandleSelect())
    {
        return;
    }
    bOperationHandleSelect = true;
    if (selectedAxisComponent)
    {
        ResetVisible(selectedAxisComponent, true);
    }
}

void GOperationHandleBase::OnLeftButtonUp(int x, int y)
{
    if (!IsCurrentOperationHandleSelect())
    {
        return;
    }
    bOperationHandleSelect = false;
    if (selectedAxisComponent)
    {
        ResetColor();
        ResetVisible();
        selectedAxisComponent = nullptr;
    }
}

void GOperationHandleBase::OnCaptureKeyboardInformation(const FInputKey& inputKey)
{
}

extern GActorObject* selectedObject;
bool GOperationHandleBase::GetRayInterHitPosition(
    int x, int y,
    float& outT1,
    fvector_3d& outWorldActorDir,
    fvector_3d& outWorldActorPosition
)
{
    ESelectAxisType axisType = GetSelectAxisType();
    if (!selectedObject || axisType == ESelectAxis_None)
    {
        return false;
    }
        
    XMVECTOR viewOrigin;
    XMVECTOR viewDir;
    XMMATRIX viewInvMatrix;
    if (FRaycastSystemLibrary::GetRaycastByScreenParam(GetWorld(), fvector_2d(x, y), viewOrigin, viewDir, viewInvMatrix))
    {
        XMVECTOR worldOriginPos = XMVector3TransformCoord(viewOrigin, viewInvMatrix);
        XMVECTOR worldDir = XMVector3TransformNormal(viewDir, viewInvMatrix);
        
        XMFLOAT3 worldOriginPosFloat3;
        XMFLOAT3 worldDirFloat3;
        XMStoreFloat3(&worldOriginPosFloat3, worldOriginPos);
        XMStoreFloat3(&worldDirFloat3, worldDir);
        
        //射线的世界方向和原点
        fvector_3d worldOriginPos3d = EngineMath::ToVector3d(worldOriginPosFloat3);
        fvector_3d worldOriginDir3d = EngineMath::ToVector3d(worldDirFloat3);
        worldOriginDir3d.normalize();
        
        outWorldActorPosition = EngineMath::ToVector3d(selectedObject->GetPosition());
        
        if (true)
        {
            switch (axisType)
            {
            case ESelectAxis_X:
                outWorldActorDir =fvector_3d(1.f, 0.f, 0.f);
                break;
            case ESelectAxis_Y:
                outWorldActorDir =fvector_3d(0.f, 1.f, 0.f);
                break;
            case ESelectAxis_Z:
                outWorldActorDir =fvector_3d(0.f, 0.f, 1.f);
                break;
            default:
                break;  
            }
        }
        else
        {
            switch (axisType)
            {
            case ESelectAxis_X:
                outWorldActorDir = EngineMath::ToVector3d(selectedObject->GetRightVector());
                break;
            case ESelectAxis_Y:
                outWorldActorDir = EngineMath::ToVector3d(selectedObject->GetUpVector());
                break;
            case ESelectAxis_Z:
                outWorldActorDir = EngineMath::ToVector3d(selectedObject->GetForwardVector());
                break;
            default:
                break;  
            }
        }
            
        fvector_3d v1Xv2 = fvector_3d::cross_product(worldOriginDir3d, outWorldActorDir);
        float len = v1Xv2.len();
            
        outT1 = fvector_3d::dot(
            fvector_3d::cross_product(outWorldActorPosition - worldOriginPos3d, worldOriginDir3d),
            v1Xv2
        ) / (len * len);
        return true;
    }
    return false;
}

bool GOperationHandleBase::IsCurrentOperationHandleSelect() const
{
    return this == FOperationHandleSelectManager::GetInstance()->GetSelectOperationHandle();
}
