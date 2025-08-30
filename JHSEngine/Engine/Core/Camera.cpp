#include "Camera.h"
#include "CameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
	inputComponent = CreateObject<CInputComponent>(new CInputComponent());
	transformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

	mounseSensitivity = .5f;
	cameraType = ECameraType::CameraRoaming;

	radius = 10.f;
	theta = 0;
	phi = 0;
}

void CCamera::BeginInit()
{
	ViewportInit();
	inputComponent->captureKeyboardInforDelegate.Bind(this, &CCamera::ExecuteKeboard);
	inputComponent->OnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
	inputComponent->OnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
	inputComponent->OnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
	inputComponent->OnMouseWheelDelegate.Bind(this, &CCamera::OnMouseWheel);
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
	if (inputKey.keyName == "T")
	{
		cameraType = ECameraType::CameraRoaming;
	}
	else if (inputKey.keyName == "Y")
	{
		cameraType = ECameraType::OvservationObject;
	}
}

void CCamera::BuildViewMatrix(float deltaTime)
{
	switch (cameraType)
	{
	case ECameraType::CameraRoaming:
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
		break;
	}
	case ECameraType::OvservationObject:
	{
		XMFLOAT3& cameraPos = transformationComponent->GetPosition();

		cameraPos.x = radius * cosf(phi) * cosf(theta);
		cameraPos.y = radius * sinf(theta);
		cameraPos.z = radius * sinf(phi) * cosf(theta);

		XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
		XMVECTOR viewTarget = XMVectorZero();
		XMVECTOR viewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

		XMMATRIX viewLookAt = XMMatrixLookAtLH(pos, viewTarget, viewUp);
		XMStoreFloat4x4(&viewMatrix, viewLookAt);
		break;
	}
	default:
		break;
	}
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
				phi += yRadians;
				break;
			}
			default:
				break;
		}
	}
	lsatMousePosition.x = x;
	lsatMousePosition.y = y;
}

void CCamera::OnMouseWheel(int x, int y, float inDelta)
{
	if (cameraType == ECameraType::OvservationObject)
	{
		radius += inDelta * -0.001f;
		radius = math_libray::Clamp(radius, 7.f, 40.0f);
	}
}

void CCamera::MoveForward(float inValue)
{
	if (cameraType == ECameraType::CameraRoaming)
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
}

void CCamera::MoveRight(float inValue)
{
	if (cameraType == ECameraType::CameraRoaming)
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
}

void CCamera::MoveUp(float inValue)
{
	if (cameraType == ECameraType::CameraRoaming)
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
