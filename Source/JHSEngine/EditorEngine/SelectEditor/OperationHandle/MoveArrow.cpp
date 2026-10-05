#include "MoveArrow.h"

#include "Common/OperationHandleSelectManager.h"
#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"

GMoveArrow::GMoveArrow()
{
}

void GMoveArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\MoveArrow.fbx";
    string customMeshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\AnyAxis_Type_1.fbx";
    
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, customAxisComponent, customMeshPath);
    
    xAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));
    yAxisComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
    
    ResetColor();
    
}

fvector_3d GMoveArrow::GetCustomAxisDirection(
    const fvector_3d inRayWorldOriginPosition,
    const fvector_3d inRayWorldDirection,
    const fvector_3d inObjectWorldPosition
) const
{
    return inRayWorldDirection;
}

extern GActorObject* selectedObject;
void GMoveArrow::OnMouseMove(int x, int y)
{
    Super::OnMouseMove(x, y);

    if (!bOperationHandleSelect || !IsCurrentOperationHandleSelect())
    {
        return;
    }
    
    float rayInterHitT1;
    fvector_3d worldActorDir;
    fvector_3d worldActorPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitT1, worldActorDir, worldActorPosition))
    {
            
        fvector_3d rayInterHitPosition = worldActorDir * rayInterHitT1 + worldActorPosition;
        XMFLOAT3 worldMovePositionFloat3 = EngineMath::ToFloat3(rayInterHitPosition + relativePosition);
        
        selectedObject->SetPosition(worldMovePositionFloat3);
        SetPosition(worldMovePositionFloat3);
        /*
        char fromPos[1024] = {0}; 
        char toPos[1024] = {0}; 
        rayInterHitPosition.to_string(fromPos);
        (rayInterHitPosition + relativePosition).to_string(toPos);
        Engine_Log("Move Object form {%s} to {%s}", fromPos, toPos);
        */
    }
}

void GMoveArrow::OnLeftButtonDown(int x, int y)
{
    Super::OnLeftButtonDown(x, y);
    
    if (!IsCurrentOperationHandleSelect())
    {
        return;
    }
    
    float rayInterHitT1;
    fvector_3d worldActorDir;
    fvector_3d worldActorPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitT1, worldActorDir, worldActorPosition))
    {
            
        fvector_3d rayInterHitPosition = worldActorDir * rayInterHitT1 + worldActorPosition;
        relativePosition = EngineMath::ToVector3d(selectedObject->GetPosition()) - rayInterHitPosition;
    }
}

void GMoveArrow::OnLeftButtonUp(int x, int y)
{
    Super::OnLeftButtonUp(x, y);
}

void GMoveArrow::OnCaptureKeyboardInformation(const FInputKey& inputKey)
{
    Super::OnCaptureKeyboardInformation(inputKey);
    if (selectedObject)
    {
        if (inputKey.keyName == "W")
        {
            FOperationHandleSelectManager::GetInstance()->DisplaySelectOperationHandle(this);
        }
    }
}
