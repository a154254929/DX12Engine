#include "ScaleArrow.h"

#include "Common/OperationHandleSelectManager.h"
#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"

GScaleArrow::GScaleArrow()
{
    fixedZoom = 35.f;
}

void GScaleArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\ScalingArrow.fbx";
    string customMeshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\AnyAxis_Type_1.fbx";
    
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, customAxisComponent, customMeshPath);
    
    xAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));
    yAxisComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
    
    ResetColor();
    
}

fvector_3d GScaleArrow::GetCustomAxisDirection(const fvector_3d inRayWorldOriginPosition,
    const fvector_3d inRayWorldDirection, const fvector_3d inObjectWorldPosition) const
{
    return fvector_3d(1.f);
}

extern GActorObject* selectedObject;
void GScaleArrow::OnMouseMove(int x, int y)
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
        fvector_3d currentScale = selectedObject->GetScale();
        float diffT1Value = rayInterHitT1 - lastT1Value;
        float scaleValue = 0.f;
        if (diffT1Value > 0.f)
        {
            scaleValue += 0.25f;
        }
        else if (diffT1Value < 0.f)
        {
            scaleValue -= 0.25f;
        }
        
        currentScale += worldActorDir * scaleValue;
        selectedObject->SetScale(currentScale);
        lastT1Value = rayInterHitT1;

    }
}

void GScaleArrow::OnLeftButtonDown(int x, int y)
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
        lastT1Value = rayInterHitT1;
    }
}

void GScaleArrow::OnLeftButtonUp(int x, int y)
{
    Super::OnLeftButtonUp(x, y);
}

void GScaleArrow::OnCaptureKeyboardInformation(const FInputKey& inputKey)
{
    Super::OnCaptureKeyboardInformation(inputKey);
    if (selectedObject)
    {
        if (inputKey.keyName == "R")
        {
            FOperationHandleSelectManager::GetInstance()->DisplaySelectOperationHandle(this);
        }
    }
}
