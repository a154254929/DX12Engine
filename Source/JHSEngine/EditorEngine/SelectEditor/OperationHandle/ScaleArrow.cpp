#include "ScaleArrow.h"

#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"

GScaleArrow::GScaleArrow()
{
    FCreateObjectParam param;
    param.owner = this;
    
    xAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    yAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    zAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    
   SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE);
}

void GScaleArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\ScalingArrow.fbx";
    
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath);
    
    xAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));
    yAxisComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
    
    ResetColor();
    
}

extern GActorObject* selectedObject;
void GScaleArrow::OnMouseMove(int x, int y)
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

void GScaleArrow::OnLeftButtonDown(int x, int y)
{
    Super::OnLeftButtonDown(x, y);
    
    fvector_3d rayInterHitPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitPosition))
    {
        relativePosition = EngineMath::ToVector3d(selectedObject->GetPosition()) - rayInterHitPosition;
    }
}

void GScaleArrow::OnLeftButtonUp(int x, int y)
{
    Super::OnLeftButtonUp(x, y);
}
