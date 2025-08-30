#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	: position(0.f, 0.f, 0.f)
	, forwardVector(0.f, 0.f, 1.f)
	, upVector(0.f, 1.f, 0.f)
	, rightVector(1.f, 0.f, 0.f)
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3& inPosition)
{
	position = inPosition;
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
	up = XMVector3Normalize(XMVector3Cross(right, forward));
	right = XMVector3Normalize(XMVector3Cross(forward, up));

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

	inPosV3.x = XMVectorGetX(XMVector3Dot(newPosition, right));
	inPosV3.y = XMVectorGetX(XMVector3Dot(newPosition, up));
	inPosV3.z = XMVectorGetX(XMVector3Dot(newPosition, forward));
}
