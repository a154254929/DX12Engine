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
