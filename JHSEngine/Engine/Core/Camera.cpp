#include "Camera.h"
#include "CameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

GCamera::GCamera()
	:GActorObject()
{
	inputComponent = CreateObject<CInputComponent>(new CInputComponent());

	mounseSensitivity = .5f;
	cameraType = ECameraType::CameraRoaming;

	radius = 10.f;
	theta = 0;
	phi = 0;
}

void GCamera::BeginInit()
{
	ViewportInit();
	inputComponent->captureKeyboardInforDelegate.Bind(this, &GCamera::ExecuteKeboard);
	inputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
	inputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
	inputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
	inputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float deltaTime)
{
	BuildViewMatrix(deltaTime);
}

void GCamera::ExecuteKeboard(const FInputKey& inputKey)
{
	if (inputKey.keyName == "W")
	{
		MoveForward(speed);
	}
	else if (inputKey.keyName == "S")
	{
		MoveForward(-speed);
	}
	if (inputKey.keyName == "D")
	{
		MoveRight(speed);
	}
	else if (inputKey.keyName == "A")
	{
		MoveRight(-speed);
	}
	if (inputKey.keyName == "E")
	{
		MoveUp(speed);
	}
	else if (inputKey.keyName == "Q")
	{
		MoveUp(-speed);
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

void GCamera::BuildViewMatrix(float deltaTime)
{
	switch (cameraType)
	{
	case ECameraType::CameraRoaming:
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
			rightVector.x,	upVector.x,		forwardVector.x,	0.f,
			rightVector.y,	upVector.y,		forwardVector.y,	0.f,
			rightVector.z,	upVector.z,		forwardVector.z,	0.f,
			v3.x,			v3.y,			v3.z,				1.f
		};
		break;
	}
	case ECameraType::OvservationObject:
	{
		XMFLOAT3& cameraPos = GetTransformationComponent()->GetPosition();

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

void GCamera::OnMouseButtonDown(int x, int y)
{
	bLeftMosueDown = true;
	SetCapture(GetMainWindowsHandle());
}

void GCamera::OnMouseButtonUp(int x, int y)
{
	bLeftMosueDown = false;
	ReleaseCapture();
}

void GCamera::OnMouseMove(int x, int y)
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
				phi -= yRadians;
				break;
			}
			default:
				break;
		}
	}
	lsatMousePosition.x = x;
	lsatMousePosition.y = y;
}

void GCamera::OnMouseWheel(int x, int y, float inDelta)
{
	if (cameraType == ECameraType::OvservationObject)
	{
		radius += inDelta * -0.001f;
		radius = math_libray::Clamp(radius, 7.f, 40.0f);
	}
}

void GCamera::MoveForward(float inValue)
{
	if (cameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 f3Position = GetTransformationComponent()->GetPosition();
		XMFLOAT3 f3Forward = GetTransformationComponent()->GetForwardVector();
		XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
		XMVECTOR forward = XMLoadFloat3(&f3Forward);
		XMVECTOR position = XMLoadFloat3(&f3Position);
		position += amountMovement * forward;
		XMStoreFloat3(&f3Position, position);
		GetTransformationComponent()->SetPosition(f3Position);
	}
}

void GCamera::MoveRight(float inValue)
{
	if (cameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 f3Position = GetTransformationComponent()->GetPosition();
		XMFLOAT3 f3Right = GetTransformationComponent()->GetRightVector();
		XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
		XMVECTOR right = XMLoadFloat3(&f3Right);
		XMVECTOR position = XMLoadFloat3(&f3Position);
		position += amountMovement * right;
		XMStoreFloat3(&f3Position, position);
		GetTransformationComponent()->SetPosition(f3Position);
	}
}

void GCamera::MoveUp(float inValue)
{
	if (cameraType == ECameraType::CameraRoaming)
	{
		XMFLOAT3 f3Position = GetTransformationComponent()->GetPosition();
		XMFLOAT3 f3Up = GetTransformationComponent()->GetUpVector();
		XMVECTOR amountMovement = XMVectorReplicate(inValue * 5.f);
		XMVECTOR up = XMLoadFloat3(&f3Up);
		XMVECTOR position = XMLoadFloat3(&f3Position);
		position += amountMovement * up;
		XMStoreFloat3(&f3Position, position);
		GetTransformationComponent()->SetPosition(f3Position);
	}
}

void GCamera::RotateAroundYAxis(float inRotateDegrees)
{
	//拿到相机方向
	XMVECTOR upVector = XMLoadFloat3(&GetTransformationComponent()->GetUpVector());

	//拿到旋转矩阵
	XMMATRIX rotationY = XMMatrixRotationY(inRotateDegrees);

	//计算各个向量旋转后的结构
	XMStoreFloat3(&GetTransformationComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()), rotationY));
	//XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(upVector, rotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&GetTransformationComponent()->GetForwardVector()), rotationY));
}

void GCamera::RotateAroundXAxis(float inRotateDegrees)
{
	//拿到相机方向
	XMVECTOR rightVector = XMLoadFloat3(&GetTransformationComponent()->GetRightVector());

	//拿到旋转矩阵
	XMMATRIX rotationX = XMMatrixRotationAxis(rightVector, inRotateDegrees);

	//计算各个向量旋转后的结构
	//XMStoreFloat3(&GetTransformationComponent()->GetRightVector(), XMVector3TransformNormal(rightVector, rotationX));
	XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&GetTransformationComponent()->GetUpVector()), rotationX));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&GetTransformationComponent()->GetForwardVector()), rotationX));

}
