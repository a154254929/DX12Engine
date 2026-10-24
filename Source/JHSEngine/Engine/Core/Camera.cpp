#include "Camera.h"
#include "CameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"
#include "../Library/RaycastSystemLibrary.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManager.h"
#include "../Component/Mesh/Core/MeshComponentType.h"
#include "Engine/Config/EngineRenderConfig.h"
#include "Engine/Math/EngineMath.h"

extern GActorObject* selectedObject;
GCamera::GCamera()
    : Super()
{
    FCreateObjectParam param;
    param.owner = this;
    inputComponent = CreateObject<CInputComponent>(param, new CInputComponent());

    mounseSensitivity = .5f;
    cameraType = ECameraType::CameraRoaming;

    radius = 10.f;
    theta = 0;
    phi = 0;
    
    bFPressed = false;
}

void GCamera::BeginInit()
{
    float aspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScreenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    SetFrustum(
        45.0f, //以弧度为单位的自上而下的视场角
        aspectRatio,//宽高比
        1.0f,//近裁剪面
        10000.f//远剪裁面
    );

    inputComponent->captureKeyboardInforDelegate.Bind(this, &GCamera::ExecuteKeboard);
    
    inputComponent->OnLeftMouseButtonDownDelegate.Bind(this, &GCamera::OnLeftMouseButtonDown);
    inputComponent->OnLeftMouseButtonUpDelegate.Bind(this, &GCamera::OnLeftMouseButtonUp);
    
    inputComponent->OnRightMouseButtonDownDelegate.Bind(this, &GCamera::OnRightMouseButtonDown);
    inputComponent->OnRightMouseButtonUpDelegate.Bind(this, &GCamera::OnRightMouseButtonUp);
    
    inputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
    inputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float deltaTime)
{
    timeline.Tick(deltaTime);
    Super::Tick(deltaTime);
}

void GCamera::ExecuteKeboard(const FInputKey& inputKey)
{
    if (inputKey.keyName == "W")
    {
        MoveForward(speed);
        
        SetDirty(true);
    }
    else if (inputKey.keyName == "S")
    {
        MoveForward(-speed);
        
        SetDirty(true);
    }
    if (inputKey.keyName == "D")
    {
        MoveRight(speed);
        
        SetDirty(true);
    }
    else if (inputKey.keyName == "A")
    {
        MoveRight(-speed);
        
        SetDirty(true);
    }
    if (inputKey.keyName == "E")
    {
        MoveUp(speed);
        
        SetDirty(true);
    }
    else if (inputKey.keyName == "Q")
    {
        MoveUp(-speed);
        
        SetDirty(true);
    }
    if (inputKey.keyName == "T")
    {
        cameraType = ECameraType::CameraRoaming;
        
        SetDirty(true);
    }
    
    if (inputKey.keyName == "alt")
    {
        cameraType = ECameraType::OvservationObject;
        
        SetDirty(true);
    }
    
    if (inputKey.keyName == "F" && !bFPressed && selectedObject)
    {
        /*
        LookAt(
            EngineMath::ToVector3d(GetPosition()),
            EngineMath::ToVector3d(selectedObject->GetPosition())
        );
        */
        
        if (cameraType == ECameraType::OvservationObject)
        {
            GetRightVector() = XMFLOAT3(viewMatrix._11, viewMatrix._21, viewMatrix._31);
            GetUpVector() = XMFLOAT3(viewMatrix._12, viewMatrix._22, viewMatrix._32);
            GetForwardVector() = XMFLOAT3(viewMatrix._13, viewMatrix._23, viewMatrix._33);
        }
        cameraType = ECameraType::CameraRoaming;
        bFPressed = true;
        timeline.ClearTimeline();
        FTimelineDelegate timelineDelegate;
        timelineDelegate.Bind(this, &GCamera::LookAtAndMoveToSelectedObject);
        timeline.BindTimelineEvent(1.0f, timelineDelegate);
    }
}

void GCamera::BuildViewMatrix(float deltaTime)
{
    switch (cameraType)
    {
        case ECameraType::CameraRoaming:
        {
            Super::BuildViewMatrix(deltaTime);
            break;
        }
        case ECameraType::OvservationObject:
        {
            XMFLOAT3& cameraPos = GetRootComponent()->GetPosition();

            cameraPos.x = radius * cosf(phi) * cosf(theta);
            cameraPos.y = radius * sinf(theta);
            cameraPos.z = radius * sinf(phi) * cosf(theta);

            XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
            XMFLOAT3& targetPosition = selectedObject->GetRootComponent()->GetPosition();
            XMVECTOR viewTarget = XMVectorSet(targetPosition.x, targetPosition.y, targetPosition.z, 1.0f);
            XMVECTOR viewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

            XMMATRIX viewLookAt = XMMatrixLookAtLH(pos, viewTarget, viewUp);
            XMStoreFloat4x4(&viewMatrix, viewLookAt);
            break;
        }
        default:
            break;
    }
}

void GCamera::OnLeftMouseButtonDown(int x, int y)
{
    bLeftMosueDown = true;
    
    OnClickedScreen(x, y);
    
    SetCapture(GetMainWindowsHandle());
}

void GCamera::OnLeftMouseButtonUp(int x, int y)
{
    bLeftMosueDown = false;
    ReleaseCapture();
}

void GCamera::OnRightMouseButtonDown(int x, int y)
{
    bRightMosueDown = true;
    
    SetCapture(GetMainWindowsHandle());
}

void GCamera::OnRightMouseButtonUp(int x, int y)
{
    bRightMosueDown = false;
    ReleaseCapture();
}

void GCamera::OnMouseMove(int x, int y)
{
    if (bRightMosueDown)
    {
        float yRadians = XMConvertToRadians(x - lsatMousePosition.x) * mounseSensitivity;
        float xRadians = XMConvertToRadians(y - lsatMousePosition.y) * mounseSensitivity;
        switch (cameraType)
        {
            case ECameraType::CameraRoaming:
            {
                RotateAroundYAxis(yRadians);
                RotateAroundXAxis(xRadians);
                break;
            }
            case ECameraType::OvservationObject:
            {
                theta += xRadians;
                theta = math_libray::Clamp(theta, -XM_PIDIV2 + 0.01f, XM_PIDIV2 - 0.01f);
                phi -= yRadians;
                break;
            }
            default:
                break;
        }
        
        SetDirty(true);
    }
    lsatMousePosition.x = x;
    lsatMousePosition.y = y;
}

void GCamera::OnMouseWheel(int x, int y, float inDelta)
{
    if (cameraType == ECameraType::OvservationObject)
    {
        radius += inDelta * -0.01f;
        radius = math_libray::Clamp(radius, 7.f, 40.0f);
        
        SetDirty(true);
    }
}

void GCamera::MoveForward(float inValue)
{
    if (cameraType == ECameraType::CameraRoaming && bRightMosueDown)
    {
        XMFLOAT3 f3Position = GetRootComponent()->GetPosition();
        XMFLOAT3 f3Forward = GetRootComponent()->GetForwardVector();
        XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
        XMVECTOR forward = XMLoadFloat3(&f3Forward);
        XMVECTOR position = XMLoadFloat3(&f3Position);
        position += amountMovement * forward;
        XMStoreFloat3(&f3Position, position);
        GetRootComponent()->SetPosition(f3Position);
    }
}

void GCamera::MoveRight(float inValue)
{
    if (cameraType == ECameraType::CameraRoaming && bRightMosueDown)
    {
        XMFLOAT3 f3Position = GetRootComponent()->GetPosition();
        XMFLOAT3 f3Right = GetRootComponent()->GetRightVector();
        XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
        XMVECTOR right = XMLoadFloat3(&f3Right);
        XMVECTOR position = XMLoadFloat3(&f3Position);
        position += amountMovement * right;
        XMStoreFloat3(&f3Position, position);
        GetRootComponent()->SetPosition(f3Position);
    }
}

void GCamera::MoveUp(float inValue)
{
    if (cameraType == ECameraType::CameraRoaming && bRightMosueDown)
    {
        XMFLOAT3 f3Position = GetRootComponent()->GetPosition();
        XMFLOAT3 f3Up = GetRootComponent()->GetUpVector();
        XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
        XMVECTOR up = XMLoadFloat3(&f3Up);
        XMVECTOR position = XMLoadFloat3(&f3Position);
        position += amountMovement * up;
        XMStoreFloat3(&f3Position, position);
        GetRootComponent()->SetPosition(f3Position);
    }
}

#if EDITOR_ENGINE
#include "../../Common/OperationHandleSelectManager.h"
extern CMeshComponent* selectedAxisComponent;
#endif
void GCamera::OnClickedScreen(int x, int y)
{
#if EDITOR_ENGINE
    if (!selectedAxisComponent)
#endif
    {
        FCollisionResult hitResult;
        FRaycastSystemLibrary::HitResultByScreen(GetWorld(), x, y, hitResult);
    
        if (hitResult.bIsCollided)
        {
            if (FRenderLayerManager* layerManager = GetRenderingLayerManager())
            {
                layerManager->HighlightDisplayObject(hitResult.renderingData);
            }
        
#if EDITOR_ENGINE
            FOperationHandleSelectManager::GetInstance()->SetNewSelectObject(hitResult.collisionActor);
            FOperationHandleSelectManager::GetInstance()->DisplaySelectOperationHandle();
#endif
       
            Engine_Log("Hit Actor! [time] = %f", hitResult.collisionTime);
        }
        else
        {
            if (FRenderLayerManager* layerManager = GetRenderingLayerManager())
            {
                layerManager->Clear(EMeshRenderLayerType::RENDERLAYER_OPAQUE_SELECT);
            }
        
#if EDITOR_ENGINE
            FOperationHandleSelectManager::GetInstance()->SetNewSelectObject(nullptr);
            FOperationHandleSelectManager::GetInstance()->HideSelectOperationHandle();
#endif
        
            Engine_Log("No Hit");
        }
    }
    
}

void GCamera::RotateAroundYAxis(float inRotateDegrees)
{
    //拿到相机方向
    XMVECTOR upVector = XMLoadFloat3(&GetRootComponent()->GetUpVector());

    //拿到旋转矩阵
    XMMATRIX rotationY = XMMatrixRotationY(inRotateDegrees);

    //计算各个向量旋转后的结构
    XMStoreFloat3(&GetRootComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&GetRootComponent()->GetRightVector()), rotationY));
    //XMStoreFloat3(&GetRootComponent()->GetUpVector(), XMVector3TransformNormal(upVector, rotationY));
    XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&GetRootComponent()->GetForwardVector()), rotationY));
}

void GCamera::RotateAroundXAxis(float inRotateDegrees)
{
    //拿到相机方向
    XMVECTOR rightVector = XMLoadFloat3(&GetRootComponent()->GetRightVector());

    //拿到旋转矩阵
    XMMATRIX rotationX = XMMatrixRotationAxis(rightVector, inRotateDegrees);

    //计算各个向量旋转后的结构
    //XMStoreFloat3(&GetRootComponent()->GetRightVector(), XMVector3TransformNormal(rightVector, rotationX));
    XMStoreFloat3(&GetRootComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&GetRootComponent()->GetUpVector()), rotationX));
    XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&GetRootComponent()->GetForwardVector()), rotationX));

}

void GCamera::LookAtAndMoveToSelectedObject(float inTime, float inDeltaTime)
{
    if (selectedObject)
    {
        BoundingBox selectedObjectBoundingBox = selectedObject->GetBoundingBox();
        float radius = EngineMath::ToVector3d(selectedObjectBoundingBox.Extents).len();
        float h = 10.f;
        float fov = XMConvertToRadians(GetYFov());
        float l = (radius + h) / tan(fov / 2);
        
        fvector_3d cameraPosition = EngineMath::ToVector3d(GetPosition());
        fvector_3d selectedObjectPosition = EngineMath::ToVector3d(selectedObject->GetPosition());
        
        fvector_3d cameraForward = selectedObjectPosition - cameraPosition;
        if (fvector_3d::dot(cameraForward, cameraForward) < 1.e-8f)
        {
            cameraForward = EngineMath::ToVector3d(GetForwardVector());
        }
        cameraForward.normalize();
        
        fvector_3d selectedObjectCenter = EngineMath::ToVector3d(selectedObjectBoundingBox.Center);
        fvector_3d endPosition = selectedObjectPosition + cameraForward * l * -1.0f;
        fvector_3d currentPosition = EngineMath::Lerp(cameraPosition, endPosition, inDeltaTime);
        
        SetPosition(EngineMath::ToFloat3(currentPosition));
        
        if (true)
        {
            fquat cameraQuat = GetRotationQuat();
            fquat targetQuat = EngineMath::BuildQuat(cameraForward);
            if ((cameraQuat | targetQuat) < 0.f)
            {
                targetQuat *= -1.f;
            }

            fquat currentQ = fquat::s_lerp(cameraQuat, targetQuat, inDeltaTime);
            currentQ.normalize();
            SetRotationQuat(currentQ);
        }
        else
        {
            frotator cameraRotator = GetRotation();
            frotator targetRotator = EngineMath::BuildRotator(cameraForward);
            frotator currentRotator = EngineMath::Lerp(cameraRotator, targetRotator, inDeltaTime);
            SetRotation(currentRotator);
        }
    }
    bFPressed = false;
}
