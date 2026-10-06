#include "RotateArrow.h"

#include "Common/OperationHandleSelectManager.h"
#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"
#include "../Engine/Core/World.h"
#include "../Engine/Core/Camera.h"

GRotateArrow::GRotateArrow()
{
    fixedZoom = 40.f;
}

void GRotateArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle";
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath + "\\RotateHandleX.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath + "\\RotateHandleY.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath + "\\RotateHandleZ.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, customAxisComponent, meshPath + "\\AnyAxis_Type_1.fbx");
    
    customAxisComponent->SetPickup(false);
    
    ResetColor();
    
}

void GRotateArrow::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    
    if (customAxisComponent)
    {
        customAxisComponent->SetScale(inScale * 1.4f);
    }
    
}

void GRotateArrow::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    
    if (!IsCurrentOperationHandleSelect())
    {
        return;
    }
    
    XMFLOAT4X4 rotateArrowMatrix;
    EngineMath::BuildMatrixFromPositionRotationScale(
        rotateArrowMatrix,
        GetPosition(),
        GetScale(),
        GetRightVector(),
        GetUpVector(),
        GetForwardVector()
    );
    
    XMMATRIX arrowWorldMatrix = XMLoadFloat4x4(&rotateArrowMatrix);
    XMVECTOR arrowWorldMatrixRIXDeterminant = XMMatrixDeterminant(arrowWorldMatrix);
    XMMATRIX arrowWorldMatrixInverse = XMMatrixInverse(&arrowWorldMatrixRIXDeterminant, arrowWorldMatrix);
    
    FXMVECTOR cameraPosition = XMLoadFloat3(&GetWorld()->GetCamera()->GetTransformationComponent()->GetPosition());
    
    XMVECTOR outObjectPosition = XMVector3TransformCoord(cameraPosition, arrowWorldMatrixInverse);
    
    XMFLOAT3 viewPosition;
    XMStoreFloat3(&viewPosition, outObjectPosition);
    
    int type = EngineMath::GetSample8CubeIndex(EngineMath::ToVector3d(viewPosition));
    
    Engine_Log("%d", type);
    
    switch (type)
    {
    case 0:
        yAxisComponent->SetRotation(frotator(0.f, -90.f, 0.f));
        xAxisComponent->SetRotation(frotator(90.f, 0.f, 0.f));
        zAxisComponent->SetRotation(frotator());
        customAxisComponent->SetRotation(frotator());
        break;
    case 1:
        yAxisComponent->SetRotation(frotator(0.f, -180.f, 0.f));
        xAxisComponent->SetRotation(frotator(90.f, 0.f, 0.f));
        zAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));
        customAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));
        break;
    case 2:
        yAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));
        xAxisComponent->SetRotation(frotator());
        zAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));
        customAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));
        break;
    case 3:
        yAxisComponent->SetRotation(frotator());
        xAxisComponent->SetRotation(frotator());
        zAxisComponent->SetRotation(frotator());
        customAxisComponent->SetRotation(frotator());
        break;
    case 4:
        yAxisComponent->SetRotation(frotator(0.f, -90.f, 0.f));
        xAxisComponent->SetRotation(frotator(-180.f, 0.f, 0.f));
        zAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
        customAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
        break;
    case 5:
        yAxisComponent->SetRotation(frotator(0.f, 180.f, 0.f));
        xAxisComponent->SetRotation(frotator(-180.f, 0.f, 0.f));
        zAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));
        customAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));
        break;
    case 6:
        yAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));
        xAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));
        zAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));
        customAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));
        break;
    case 7:
        yAxisComponent->SetRotation(frotator());
        xAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));
        zAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
        customAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
        break;
    }

}

extern GActorObject* selectedObject;
void GRotateArrow::OnMouseMove(int x, int y)
{
    Super::OnMouseMove(x, y);

    if (!selectedObject || !bOperationHandleSelect || !IsCurrentOperationHandleSelect())
    {
        return;
    }
    
    float rayInterHitT1;
    fvector_3d worldActorDir;
    fvector_3d worldActorPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitT1, worldActorDir, worldActorPosition))
    {
        float diffT1Value = rayInterHitT1 - lastT1Value;
        float rotateValue = 0.f;
        
        if (diffT1Value > 0.f)
        {
            rotateValue += 2.5f;
        }
        else if (diffT1Value < 0.f)
        {
            rotateValue -= 2.5f;
        }
        lastT1Value = rayInterHitT1;

        if (rotateValue == 0.f)
        {
            return;
        }
        
        fvector_3d deltaVector = worldActorDir * rotateValue;
        
        fvector_3d lastRotation = selectedObject->GetRotation();
        frotator lastRotator(lastRotation.y, lastRotation.z, lastRotation.x);
        frotator deltaRotator(deltaVector.y, deltaVector.z, deltaVector.x);
        
        fquat actorRotationQuat;
        fquat deltaRotationQuat;
        // Use the same rotation convention as resultRotator.object_to_inertia below.
        actorRotationQuat.object_to_inertia(lastRotator);
        deltaRotationQuat.object_to_inertia(deltaRotator);
        fquat resultRotationQuat;
        
        if (false)
        {
            
        }
        else
        {
            resultRotationQuat = actorRotationQuat * deltaRotationQuat;
            resultRotationQuat.normalize();
        }
        
        frotator resultRotator;
        resultRotator.object_to_inertia(resultRotationQuat);
        
        fvector_3d resultRotation(resultRotator.roll, resultRotator.pitch, resultRotator.yaw);
        
        selectedObject->SetRotation(resultRotation);
        
    }
}

void GRotateArrow::OnLeftButtonDown(int x, int y)
{
    Super::OnLeftButtonDown(x, y);
    
    if (!selectedObject || !IsCurrentOperationHandleSelect() || GetSelectAxisType() == ESelectAxis_None)
    {
        return;
    }
    
    float rayInterHitT1;
    fvector_3d worldActorDir;
    fvector_3d worldActorPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitT1, worldActorDir, worldActorPosition))
    {
        lastT1Value = rayInterHitT1;
    }
}

void GRotateArrow::OnLeftButtonUp(int x, int y)
{
    Super::OnLeftButtonUp(x, y);
    bOperationHandleSelect = false;
}

void GRotateArrow::OnCaptureKeyboardInformation(const FInputKey& inputKey)
{
    Super::OnCaptureKeyboardInformation(inputKey);
    if (selectedObject)
    {
        if (inputKey.keyName == "E")
        {
            FOperationHandleSelectManager::GetInstance()->DisplaySelectOperationHandle(this);
        }
    }
}
