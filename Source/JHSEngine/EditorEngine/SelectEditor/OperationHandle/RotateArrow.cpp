#include "RotateArrow.h"

#include "Common/OperationHandleSelectManager.h"
#include "Engine/Core/Consttruction/MacroConstruction.h"
#include "Engine/Library/RaycastSystemLibrary.h"
#include "../Engine/Core/World.h"
#include "../Engine/Core/Camera.h"
#include "Engine/Component/Mesh/PlaneMeshComponent.h"
#include "Engine/Mesh/Core/Material/Material.h"

GRotateArrow::GRotateArrow()
    : Super()
{
    fixedZoom = 20.f;
    
    FCreateObjectParam param;
    param.owner = this;
    xPlaneComponent = ConstructionObject<CPlaneMeshComponent>(param);
    yPlaneComponent = ConstructionObject<CPlaneMeshComponent>(param);
    zPlaneComponent = ConstructionObject<CPlaneMeshComponent>(param);
    
    ignoreComponents.push_back(xPlaneComponent);
    ignoreComponents.push_back(yPlaneComponent);
    ignoreComponents.push_back(zPlaneComponent);
    
    xPlaneComponent->SetRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE_ROT_PLANE);
    yPlaneComponent->SetRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE_ROT_PLANE);
    zPlaneComponent->SetRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPERATION_HANDLE_ROT_PLANE);
    
    lastT1Value = 0.f;
    rotateRate = 0.f;
}

void GRotateArrow::SetMeshRenderLayerType(EMeshRenderLayerType inRenderLayerType)
{
    Super::SetMeshRenderLayerType(inRenderLayerType);
}

void GRotateArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle";
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath + "\\RotateHandleX.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath + "\\RotateHandleY.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath + "\\RotateHandleZ.fbx");
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, customAxisComponent, meshPath + "\\AnyAxis_Type_1.fbx");
    
    CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, xPlaneComponent, 5.2f, 5.2f, 2, 2);
    CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, yPlaneComponent, 5.2f, 5.2f, 2, 2);
    CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, zPlaneComponent, 5.2f, 5.2f, 2, 2);
    
    customAxisComponent->SetPickup(false);
    
    xPlaneComponent->SetRotation(frotator(0.f, 0.f, 90.f));
    yPlaneComponent->SetRotation(frotator(0.f, 0.f, 0.f));
    zPlaneComponent->SetRotation(frotator(90.f, 0.f, 0.f));
    
    std::string planeComponentTextureFilename = "Rot_Handle_Plane.dds";
    LoadPlaneComponentMaterial(xPlaneComponent, planeComponentTextureFilename, fvector_3d(1.f, 0.f, 0.f));
    LoadPlaneComponentMaterial(yPlaneComponent, planeComponentTextureFilename, fvector_3d(0.f, 1.f, 0.f));
    LoadPlaneComponentMaterial(zPlaneComponent, planeComponentTextureFilename, fvector_3d(0.f, 0.f, 1.f));
    
    ResetColor();
    
}

void GRotateArrow::LoadPlaneComponentMaterial(
    CMeshComponent* inMeshComponent,
    const std::string& inFilename,
    const fvector_3d& inBaseColor
)
{
    if (CMaterial* meshMaterial = (*inMeshComponent->GetMaterials())[0])
    {
        meshMaterial->SetBaseColor(inBaseColor);
        meshMaterial->SetBaseColorIndexKey(inFilename);
    }
}

void GRotateArrow::SetCDValue(CMeshComponent* inMeshComponent, float inCDValue)
{
    if (CMaterial* meshMaterial = (*inMeshComponent->GetMaterials())[0])
    {
        meshMaterial->SetFloatCustomParam(0, inCDValue);
    }
}

void GRotateArrow::SetCDValue(float inCDValue)
{
    SetCDValue(xPlaneComponent, inCDValue);
    SetCDValue(yPlaneComponent, inCDValue);
    SetCDValue(zPlaneComponent, inCDValue);
}

void GRotateArrow::ResetCDValue()
{
    SetCDValue(0.f);
}

void GRotateArrow::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    
    if (customAxisComponent)
    {
        customAxisComponent->SetScale(inScale * 1.4f);
    }
    xPlaneComponent->SetScale(inScale);
    yPlaneComponent->SetScale(inScale);
    zPlaneComponent->SetScale(inScale);
}

void GRotateArrow::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    
    if (customAxisComponent)
    {
        customAxisComponent->SetPosition(inPosition);
    }
    xPlaneComponent->SetPosition(inPosition);
    yPlaneComponent->SetPosition(inPosition);
    zPlaneComponent->SetPosition(inPosition);
}

void GRotateArrow::ResetVisible()
{
    Super::ResetVisible();
    xPlaneComponent->SetVisible(false);
    yPlaneComponent->SetVisible(false);
    zPlaneComponent->SetVisible(false);
}

void GRotateArrow::ResetVisible(CMeshComponent* inAxisComponent, bool inVisible)
{
    Super::ResetVisible(inAxisComponent, inVisible);
    GOperationHandleBase::ESelectAxisType axisType = GetSelectAxisType();
    if (axisType == GOperationHandleBase::ESelectAxisType::ESelectAxis_X)
    {
        xPlaneComponent->SetVisible(inVisible);
    }
    else if (axisType == GOperationHandleBase::ESelectAxisType::ESelectAxis_Y)
    {
        yPlaneComponent->SetVisible(inVisible);
    }
    else if (axisType == GOperationHandleBase::ESelectAxisType::ESelectAxis_Z)
    {
        zPlaneComponent->SetVisible(inVisible);
    }
}

extern GActorObject* selectedObject;
fvector_3d GRotateArrow::GetSelectedObjectDirection(
    const fvector_3d& inRayWorldOriginPosition,
    const fvector_3d& inRayWorldDirection,
    const fvector_3d& inObjectWorldPosition)
{
    ESelectAxisType axisType = GetSelectAxisType();
        
    fvector_3d outWorldActorDirection;
    if (true)
    {
        switch (axisType)
        {
        case ESelectAxis_X:
            {
                //outWorldActorDirection = fvector_3d(0.f, 1.f, 0.f);
                outWorldActorDirection = EngineMath::ToVector3d(GetWorld()->GetCamera()->GetUpVector());
            }
            break;
        case ESelectAxis_Y:
            {
                //outWorldActorDirection = fvector_3d(1.f, 0.f, 0.f);
                outWorldActorDirection = EngineMath::ToVector3d(GetWorld()->GetCamera()->GetRightVector());
            }
            break;
        case ESelectAxis_Z:
            {
                //outWorldActorDirection = fvector_3d(0.f, 1.f, 0.f);
                outWorldActorDirection = EngineMath::ToVector3d(GetWorld()->GetCamera()->GetUpVector());
            }
            break;
        case ESelectAxis_Any:
            outWorldActorDirection = GetCustomAxisDirection(
                inRayWorldOriginPosition,
                inRayWorldDirection,
                inObjectWorldPosition
            );
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
            outWorldActorDirection = EngineMath::ToVector3d(selectedObject->GetUpVector());
            break;
        case ESelectAxis_Y:
            outWorldActorDirection = EngineMath::ToVector3d(selectedObject->GetRightVector());
            break;
        case ESelectAxis_Z:
            outWorldActorDirection = EngineMath::ToVector3d(selectedObject->GetUpVector());
            break;
        case ESelectAxis_Any:
            outWorldActorDirection = GetCustomAxisDirection(
                inRayWorldOriginPosition,
                inRayWorldDirection,
                inObjectWorldPosition
            );
            break;
        default:
            break;  
        }
    }
    return outWorldActorDirection;
}

void GRotateArrow::BeginInit()
{
    GOperationHandleBase::BeginInit();
}

void GRotateArrow::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    
    if (!IsCurrentOperationHandleSelect())
    {
        return;
    }
    
    XMMATRIX arrowWorldMatrixInverse;
    EngineMath::BuildMatrixInvFromPositionRotationScale(
        arrowWorldMatrixInverse,
        GetPosition(),
        GetScale(),
        GetRightVector(),
        GetUpVector(),
        GetForwardVector()
    );
    
    FXMVECTOR cameraPosition = XMLoadFloat3(&GetWorld()->GetCamera()->GetRootComponent()->GetPosition());
    
    XMVECTOR outObjectPosition = XMVector3TransformCoord(cameraPosition, arrowWorldMatrixInverse);
    
    XMFLOAT3 viewPosition;
    XMStoreFloat3(&viewPosition, outObjectPosition);
    
    sample8CubeIndex = EngineMath::GetSample8CubeIndex(EngineMath::ToVector3d(viewPosition));
    
    Engine_Log("%d", sample8CubeIndex);
    
    switch (sample8CubeIndex)
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

void GRotateArrow::SetVisible(bool inVisible)
{
    Super::SetVisible(inVisible);
    
    xPlaneComponent->SetVisible(false);
    yPlaneComponent->SetVisible(false);
    zPlaneComponent->SetVisible(false);
}

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
        
        //保证摄像机和物体的相对拖拽偏移保持不变
        float cmaeraAndSelectedDistance = 
            (EngineMath::ToVector3d(GetWorld()->GetCamera()->GetPosition())
            - EngineMath::ToVector3d(selectedObject->GetPosition())).len();
        float deltaRotate = (rayInterHitT1 - lastT1Value) / cmaeraAndSelectedDistance;

        //float diffT1Value = rayInterHitT1 - lastT1Value;
        float diffT1Value = deltaRotate - rotateRate;
        float rotateSymbol = GetSymbolByCubeIndex(diffT1Value);
        
        rotateRate = deltaRotate;

        if (rotateSymbol == 0.f)
        {
            return;
        }
        
        // The ray calculation uses a drag direction; rotation uses the selected axis.
        worldActorDir = GOperationHandleBase::GetSelectedObjectDirection(
            fvector_3d(),
            fvector_3d(),
            fvector_3d()
        );
        
        //模型旋转
        fvector_3d deltaVector = worldActorDir * rotateSymbol * fabsf(diffT1Value) * 360.f;
        
        //材质旋转
        float mateirlSymbol = GetSymbolMaterialByCubeIndex(diffT1Value);
        SetCDValue(rotateRate * mateirlSymbol);
        
        
        frotator lastRotator;
        frotator deltaRotator(deltaVector.y, deltaVector.z, deltaVector.x);
        
        fquat actorRotationQuat;
        fquat deltaRotationQuat;
        // Use the same rotation convention as resultRotator.object_to_inertia below.
        actorRotationQuat.object_to_inertia(lastRotator);
        deltaRotationQuat.object_to_inertia(deltaRotator);
        fquat resultRotationQuat;
        resultRotationQuat = actorRotationQuat * deltaRotationQuat;
        resultRotationQuat.normalize();
        
        frotator resultRotator;
        resultRotator.object_to_inertia(resultRotationQuat);
        
        fvector_3d resultRotation(resultRotator.roll, resultRotator.pitch, resultRotator.yaw);
        
        if (false)
        {
            XMMATRIX worldInvMatrix;
            EngineMath::BuildMatrixInvFromPositionRotationScale(
                worldInvMatrix,
                selectedObject->GetPosition(),
                selectedObject->GetScale(),
                selectedObject->GetRightVector(),
                selectedObject->GetUpVector(),
                selectedObject->GetForwardVector()
            );
            
            XMFLOAT3 xmSelectedObjectRot = EngineMath::ToFloat3(resultRotation);
            FXMVECTOR selectedObjectRotation = XMLoadFloat3(&xmSelectedObjectRot);
            
            XMVECTOR outSelectedRot = XMVector3TransformNormal(selectedObjectRotation, worldInvMatrix);
            
            XMFLOAT3 selectViewRot;
            XMStoreFloat3(&selectViewRot, outSelectedRot);
            
            
            resultRotation = EngineMath::ToVector3d(selectViewRot);
        }
        else
        {
        }
        
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
    ResetCDValue();
    if (GetRayInterHitPosition(x, y, rayInterHitT1, worldActorDir, worldActorPosition))
    {
        lastT1Value = rayInterHitT1;
        rotateRate = 0.f;
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
    if (selectedObject
        && !bRightMouseButtonDown
        && !IsCurrentOperationHandleSelect()
        && inputKey.keyName == "E"
    )
    {
        FOperationHandleSelectManager::GetInstance()->DisplaySelectOperationHandle(this);
        
        ResetVisible();
    }
}

float GRotateArrow::GetSymbol(float inValueOffset, bool bFlip)
{
    if (inValueOffset > 0.f)
    {
        return bFlip ? 1.f : -1.f;
    }
    else if (inValueOffset < 0.f)
    {
        return bFlip ? -1.f : 1.f;
    }
    return 0.f;
}

float GRotateArrow::GetSymbolByCubeIndex(float inValueOffset)
{
    GOperationHandleBase::ESelectAxisType axisType = GetSelectAxisType();
        
    switch (sample8CubeIndex)
    {
    case 0:
    case 1:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
                {
                    return GetSymbol(inValueOffset);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Y:
                {
                    return GetSymbol(inValueOffset);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                {
                    return GetSymbol(inValueOffset, sample8CubeIndex == 0);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Any:
                break;
            }
        }
        break;
    case 2:
    case 3:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
                {
                    return GetSymbol(inValueOffset, true);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Y:
                {
                    return GetSymbol(inValueOffset);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                {
                    return GetSymbol(inValueOffset, sample8CubeIndex == 3);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Any:
                break;
            }
        }
        break;
    case 4:
    case 5:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
                {
                    return GetSymbol(inValueOffset);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Y:
                {
                    return GetSymbol(inValueOffset);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                {
                    return GetSymbol(inValueOffset, sample8CubeIndex == 4);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Any:
                break;
            }    
        }
        break;
    case 6:
    case 7:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
                {
                    return GetSymbol(inValueOffset, true);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Y:
                {
                    return GetSymbol(inValueOffset);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                {
                    return GetSymbol(inValueOffset, sample8CubeIndex == 7);
                }
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Any:
                break;
            }    
        }
        break;
    }
    
    return 0.f;
}

float GRotateArrow::GetSymbolMaterialByCubeIndex(float inValueOffset)
{
    GOperationHandleBase::ESelectAxisType axisType = GetSelectAxisType();
        
    switch (sample8CubeIndex)
    {
    case 2:
    case 3:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Y:
                return 1.f;
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                return sample8CubeIndex == 3 ? -1.f : 1.f;
                break;
            }
        }
        break;
    case 0:
    case 1:
    case 4:
    case 5:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
                return -1.f;
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                return sample8CubeIndex == 0 || sample8CubeIndex == 4 ? -1.f : 1.f;
                break;
            }    
        }
        break;
    case 6:
    case 7:
        {
            switch (axisType)
            {
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_X:
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Y:
                return 1.f;
                break;
            case GOperationHandleBase::ESelectAxisType::ESelectAxis_Z:
                return sample8CubeIndex == 7 ? -1.f : 1.f;
                break;
            }    
        }
        break;
    }
    
    return 1.f;
}
