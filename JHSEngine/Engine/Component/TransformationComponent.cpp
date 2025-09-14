#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	: position(0.f, 0.f, 0.f)
	, rotation(0.f, 0.f, 0.f)
	, scale(1.f, 1.f, 1.f)
	, forwardVector(0.f, 0.f, 1.f)
	, upVector(0.f, 1.f, 0.f)
	, rightVector(1.f, 0.f, 0.f)
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3& inPosition)
{
	position = inPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& inRotation)
{
	float rollRadians = XMConvertToRadians(inRotation.z);
	float pitchRadians = XMConvertToRadians(inRotation.x);
	float yawRadians = XMConvertToRadians(inRotation.y);

	XMMATRIX rotationPitchYawRoll = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);

	XMVECTOR right = XMLoadFloat3(&rightVector);
	XMVECTOR up = XMLoadFloat3(&upVector);
	XMVECTOR forward = XMLoadFloat3(&forwardVector);

	XMStoreFloat3(&rightVector, XMVector3TransformNormal(XMLoadFloat3(&rightVector), rotationPitchYawRoll));
	XMStoreFloat3(&upVector, XMVector3TransformNormal(XMLoadFloat3(&upVector), rotationPitchYawRoll));
	XMStoreFloat3(&forwardVector, XMVector3TransformNormal(XMLoadFloat3(&forwardVector), rotationPitchYawRoll));
}

void CTransformationComponent::SetScale(const fvector_3d& inScale)
{
	scale.x = inScale.x;
	scale.y = inScale.y;
	scale.z = inScale.z;
}

void CTransformationComponent::SetForwardVector(const XMFLOAT3& inForwardVector)
{
	forwardVector = inForwardVector;
}

void CTransformationComponent::SetRightVector(const XMFLOAT3& inRightVector)
{
	rightVector = inRightVector;
}

void CTransformationComponent::SetUpVector(const XMFLOAT3& inUpVector)
{
	upVector = inUpVector;
}

void CTransformationComponent::CorrectionVector()
{
	XMVECTOR right = XMLoadFloat3(&rightVector);
	XMVECTOR up = XMLoadFloat3(&upVector);
	XMVECTOR forward = XMLoadFloat3(&forwardVector);

	forward = XMVector3Normalize(forward);
	up = XMVector3Normalize(XMVector3Cross(forward, right));
	right = XMVector3Normalize(XMVector3Cross(up, forward));

	XMStoreFloat3(&rightVector, right);
	XMStoreFloat3(&upVector, up);
	XMStoreFloat3(&forwardVector, forward);
}

fvector_3d CTransformationComponent::GetCorrectionPosition()
{
	fvector_3d f3D;
	GetCorrectionPosition(f3D);
	return f3D;
}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& inPosV3)
{
	XMVECTOR right = XMLoadFloat3(&rightVector);
	XMVECTOR up = XMLoadFloat3(&upVector);
	XMVECTOR forward = XMLoadFloat3(&forwardVector);

	XMVECTOR newPosition = XMLoadFloat3(&position);

	inPosV3.x = -XMVectorGetX(XMVector3Dot(newPosition, right));
	inPosV3.y = -XMVectorGetX(XMVector3Dot(newPosition, up));
	inPosV3.z = -XMVectorGetX(XMVector3Dot(newPosition, forward));
}
