#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
	inputComponent = CreateObject<CInputComponent>(new CInputComponent());
	transformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
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
}

void CCamera::OnMouseButtonDown(int x, int y)
{
	lsatMousePosition.x = x;
	lsatMousePosition.y = y;
}

void CCamera::OnMouseButtonUp(int x, int y)
{

}

void CCamera::OnMouseMove(int x, int y)
{

}

void CCamera::MoveForward(float inValue)
{
}

void CCamera::MoveRight(float inValue)
{
}
