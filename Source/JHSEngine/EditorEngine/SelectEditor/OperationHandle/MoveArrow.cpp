#include "MoveArrow.h"

#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"

GMoveArrow::GMoveArrow()
{
    FCreateObjectParam param;
    param.owner = this;
    
    xAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    yAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    zAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    
   SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE);
}

void GMoveArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\MoveArrow.fbx";
    
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath);
    
    xAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));
    yAxisComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
    
    ResetColor();
    
}

extern GActorObject* selectedObject;
void GMoveArrow::OnMouseMove(int x, int y)
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

void GMoveArrow::OnLeftButtonDown(int x, int y)
{
    Super::OnLeftButtonDown(x, y);
    
    fvector_3d rayInterHitPosition;
    if (GetRayInterHitPosition(x, y, rayInterHitPosition))
    {
        relativePosition = EngineMath::ToVector3d(selectedObject->GetPosition()) - rayInterHitPosition;
    }
}

void GMoveArrow::OnLeftButtonUp(int x, int y)
{
    Super::OnLeftButtonUp(x, y);
}

bool GMoveArrow::GetRayInterHitPosition(int x, int y, fvector_3d& outHitPosition)
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
        
        fvector_3d worldActorPosition = EngineMath::ToVector3d(selectedObject->GetPosition());
        fvector_3d worldActorDir;
        switch (axisType)
        {
        case ESelectAxis_X:
            worldActorDir = EngineMath::ToVector3d(selectedObject->GetRightVector());
            break;
        case ESelectAxis_Y:
            worldActorDir = EngineMath::ToVector3d(selectedObject->GetUpVector());
            break;
        case ESelectAxis_Z:
            worldActorDir = EngineMath::ToVector3d(selectedObject->GetForwardVector());
            break;
        default:
            break;  
        }
            
        fvector_3d v1Xv2 = fvector_3d::cross_product(worldOriginDir3d, worldActorDir);
        float len = v1Xv2.len();
            
        float t1 = fvector_3d::dot(
            fvector_3d::cross_product(worldActorPosition - worldOriginPos3d, worldOriginDir3d),
            v1Xv2
        ) / (len * len);
            
        outHitPosition = worldActorDir * t1 + worldActorPosition;
        return true;
    }
    return false;
}
