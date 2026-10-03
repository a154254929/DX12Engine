#include "RotateArrow.h"

#include "Common/OperationHandleSelectManager.h"
#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"

GRotateArrow::GRotateArrow()
{
}

void GRotateArrow::CreateMesh()
{
    
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\RotateHandleX.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\RotateHandleY.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\RotateHandleZ.fbx");
    
    xAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));
    yAxisComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
    
    ResetColor();
    
}

extern GActorObject* selectedObject;
void GRotateArrow::OnMouseMove(int x, int y)
{
    Super::OnMouseMove(x, y);

    if (!bOperationHandleSelect)
    {
        return;
    }
    
    fvector_3d rayInterHitPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitPosition))
    {
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

void GRotateArrow::OnLeftButtonDown(int x, int y)
{
    Super::OnLeftButtonDown(x, y);
    
    fvector_3d rayInterHitPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitPosition))
    {
        relativePosition = EngineMath::ToVector3d(selectedObject->GetPosition()) - rayInterHitPosition;
    }
}

void GRotateArrow::OnLeftButtonUp(int x, int y)
{
    Super::OnLeftButtonUp(x, y);
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
