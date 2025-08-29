#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
}

void CInputComponent::Tick(float deltaTime)
{
	if (captureKeyboardInforDelegate.IsBound())
	{
		FInputKey inputKey;
		if (GetAsyncKeyState('W') & 0x8000)
		{
			inputKey.keyName = "W";
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			inputKey.keyName = "S";
		}
		else if (GetAsyncKeyState('A') & 0x8000)
		{
			inputKey.keyName = "A";
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			inputKey.keyName = "D";
		}
		else
		{
			return;
		}
		captureKeyboardInforDelegate.Execute(inputKey);
	}
}

void CInputComponent::OnMouseButtonDown(int x, int y)
{
	if (OnMouseButtonDownDelegate.IsBound())
	{
		OnMouseButtonDownDelegate.Execute(x, y);
	}
}

void CInputComponent::OnMouseButtonUp(int x, int y)
{
	if (OnMouseButtonUpDelegate.IsBound())
	{
		OnMouseButtonUpDelegate.Execute(x, y);
	}
}

void CInputComponent::OnMouseMove(int x, int y)
{
	if (OnMouseMoveDelegate.IsBound())
	{
		OnMouseMoveDelegate.Execute(x, y);
	}
}

