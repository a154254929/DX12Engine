#include "ClientViewport.h"
#include "../../Component/TransformationComponent.h"
#include "../../Math/EngineMath.h"

GClientViewport::GClientViewport()
    : SuperV()
    , SuperA()
    , yFov(0.f)
    , aspect(0.f)
    , zNear(0.f)
    , zFar(0.f)
    , bDirty(true)
{
}

void GClientViewport::SetFrustum(float inYFov, float inAspect, float inZNear, float inZFar)
{
    yFov = inYFov;
    aspect = inAspect;
    zNear = inZNear;
    zFar = inZFar;
    
    XMMATRIX project = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(yFov),
        aspect,
        zNear,
        zFar
    );
    
    XMStoreFloat4x4(&projectMatrix, project);
    
    SetDirty(true);
}

bool GClientViewport::LookAt(const fvector_3d& inPosition, const fvector_3d& inTargetPosition, const fvector_3d& inUp)
{
    fvector_3d forwardVector = inTargetPosition - inPosition;
    if (fvector_3d::dot(forwardVector, forwardVector) == 0.f)
    {
        return false;
    }
    forwardVector.normalize();
    fvector_3d rightVector = fvector_3d::cross_product(inUp, forwardVector);
    rightVector.normalize();
    fvector_3d upVector = fvector_3d::cross_product(forwardVector, rightVector);
    upVector.normalize();
    
    GetForwardVector() = EngineMath::ToFloat3(forwardVector);
    GetRightVector() = EngineMath::ToFloat3(rightVector);
    GetUpVector() = EngineMath::ToFloat3(upVector);
    
    SetDirty(true);
    
    return true;
}

void GClientViewport::Tick(float deltaTime)
{
    BuildViewMatrix(deltaTime);
}

void GClientViewport::BuildViewMatrix(float deltaTime)
{
    //计算和矫正
    GetTransformationComponent()->CorrectionVector();

    //计算自身方向移动的意图
    fvector_3d v3 = GetTransformationComponent()->GetCorrectionPosition();

    //构建ViewMatrix
    XMFLOAT3 rightVector = GetTransformationComponent()->GetRightVector();
    XMFLOAT3 upVector = GetTransformationComponent()->GetUpVector();
    XMFLOAT3 forwardVector = GetTransformationComponent()->GetForwardVector();

    viewMatrix = {
        rightVector.x,    upVector.x,        forwardVector.x,    0.f,
        rightVector.y,    upVector.y,        forwardVector.y,    0.f,
        rightVector.z,    upVector.z,        forwardVector.z,    0.f,
        v3.x,            v3.y,            v3.z,                1.f
    };
}
