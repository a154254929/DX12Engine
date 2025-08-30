#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
	inputComponent = CreateObject<CInputComponent>(new CInputComponent());
	transformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

	mounseSensitivity = .5f;
}

void CCamera::BeginInit()
{
	ViewportInit();
	inputComponent->captureKeyboardInforDelegate.Bind(this, &CCamera::ExecuteKeboard);
	inputComponent->OnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
	inputComponent->OnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
	inputComponent->OnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
}

void CCamera::Tick(float deltaTime)
{
	BuildViewMatrix(deltaTime);
}

void CCamera::ExecuteKeboard(const FInputKey& inputKey)
{
	if (inputKey.keyName == "W")
	{
		MoveForward(1.f);
	}
	else if (inputKey.keyName == "S")
	{
		MoveForward(-1.f);
	}
	if (inputKey.keyName == "D")
	{
		MoveRight(1.f);
	}
	else if (inputKey.keyName == "A")
	{
		MoveRight(-1.f);
	}
	if (inputKey.keyName == "E")
	{
		MoveUp(1.f);
	}
	else if (inputKey.keyName == "Q")
	{
		MoveUp(-1.f);
	}
}

void CCamera::BuildViewMatrix(float deltaTime)
{
	//计算和矫正
	transformationComponent->CorrectionVector();

	//计算自身方向移动的意图
	fvector_3d v3 = transformationComponent->GetCorrectionPosition();

	//构建ViewMatrix
	XMFLOAT3 rightVector = transformationComponent->GetRightVector();
	XMFLOAT3 upVector = transformationComponent->GetUpVector();
	XMFLOAT3 forwardVector = transformationComponent->GetForwardVector();

	viewMatrix = {
		rightVector.x,	upVector.x,		forwardVector.x,	0.f,
		rightVector.y,	upVector.y,		forwardVector.y,	0.f,
		rightVector.z,	upVector.z,		forwardVector.z,	0.f,
		v3.x,			v3.y,			v3.z,				1.f
	};
}

void CCamera::OnMouseButtonDown(int x, int y)
{
	bLeftMosueDown = true;
	SetCapture(GetMainWindowsHandle());
}

void CCamera::OnMouseButtonUp(int x, int y)
{
	bLeftMosueDown = false;
	ReleaseCapture();
}

void CCamera::OnMouseMove(int x, int y)
{
	if (bLeftMosueDown)
	{
		float yRadians = XMConvertToRadians(x - lsatMousePosition.x) * mounseSensitivity;
		float xRadians = XMConvertToRadians(y - lsatMousePosition.y) * mounseSensitivity;
		RotateAroundYAxis(yRadians);
		RotateAroundXAxis(xRadians);
	}
	lsatMousePosition.x = x;
	lsatMousePosition.y = y;
}

void CCamera::MoveForward(float inValue)
{
	XMFLOAT3 f3Position = transformationComponent->GetPosition();
	XMFLOAT3 f3Forward = transformationComponent->GetForwardVector();
	XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
	XMVECTOR forward = XMLoadFloat3(&f3Forward);
	XMVECTOR position = XMLoadFloat3(&f3Position);
	position += amountMovement * forward;
	XMStoreFloat3(&f3Position, position);
	transformationComponent->SetPosition(f3Position);
}

void CCamera::MoveRight(float inValue)
{
	XMFLOAT3 f3Position = transformationComponent->GetPosition();
	XMFLOAT3 f3Right = transformationComponent->GetRightVector();
	XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
	XMVECTOR right = XMLoadFloat3(&f3Right);
	XMVECTOR position = XMLoadFloat3(&f3Position);
	position += amountMovement * right;
	XMStoreFloat3(&f3Position, position);
	transformationComponent->SetPosition(f3Position);
}

void CCamera::MoveUp(float inValue)
{
	XMFLOAT3 f3Position = transformationComponent->GetPosition();
	XMFLOAT3 f3Up = transformationComponent->GetUpVector();
	XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
	XMVECTOR up = XMLoadFloat3(&f3Up);
	XMVECTOR position = XMLoadFloat3(&f3Position);
	position += amountMovement * up;
	XMStoreFloat3(&f3Position, position);
	transformationComponent->SetPosition(f3Position);
}

void CCamera::RotateAroundYAxis(float inRotateDegrees)
{
	//拿到相机方向
	XMVECTOR upVector = XMLoadFloat3(&transformationComponent->GetUpVector());

	//拿到旋转矩阵
	XMMATRIX rotationY = XMMatrixRotationY(inRotateDegrees);

	//计算各个向量旋转后的结构
	XMStoreFloat3(&transformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&transformationComponent->GetRightVector()), rotationY));
	//XMStoreFloat3(&transformationComponent->GetUpVector(), XMVector3TransformNormal(upVector, rotationY));
	XMStoreFloat3(&transformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&transformationComponent->GetForwardVector()), rotationY));
}

void CCamera::RotateAroundXAxis(float inRotateDegrees)
{
	//拿到相机方向
	XMVECTOR rightVector = XMLoadFloat3(&transformationComponent->GetRightVector());

	//拿到旋转矩阵
	XMMATRIX rotationX = XMMatrixRotationAxis(rightVector, inRotateDegrees);

	//计算各个向量旋转后的结构
	//XMStoreFloat3(&transformationComponent->GetRightVector(), XMVector3TransformNormal(rightVector, rotationX));
	XMStoreFloat3(&transformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&transformationComponent->GetUpVector()), rotationX));
	XMStoreFloat3(&transformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&transformationComponent->GetForwardVector()), rotationX));

}
