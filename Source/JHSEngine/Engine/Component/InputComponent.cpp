#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
    LeftMouseDownDelegate.AddFunction(this, &CInputComponent::OnLeftMouseButtonDown);
    LeftMouseUpDelegate.AddFunction(this, &CInputComponent::OnLeftMouseButtonUp);
    RightMouseDownDelegate.AddFunction(this, &CInputComponent::OnRightMouseButtonDown);
    RightMouseUpDelegate.AddFunction(this, &CInputComponent::OnRightMouseButtonUp);
    
    MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
    MouseWheelDelegate.AddFunction(this, &CInputComponent::OnMouseWheel);
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
        else if (GetAsyncKeyState('E') & 0x8000)
        {
            inputKey.keyName = "E";
        }
        else if (GetAsyncKeyState('Q') & 0x8000)
        {
            inputKey.keyName = "Q";
        }
        else if (GetAsyncKeyState('T') & 0x8000)
        {
            inputKey.keyName = "T";
        }
        else if (GetAsyncKeyState('Y') & 0x8000)
        {
            inputKey.keyName = "Y";
        }
        else
        {
            return;
        }
        captureKeyboardInforDelegate.Execute(inputKey);
    }
}

void CInputComponent::OnLeftMouseButtonDown(int x, int y)
{
    if (OnLeftMouseButtonDownDelegate.IsBound())
    {
        OnLeftMouseButtonDownDelegate.Execute(x, y);
    }
}

void CInputComponent::OnLeftMouseButtonUp(int x, int y)
{
    if (OnLeftMouseButtonUpDelegate.IsBound())
    {
        OnLeftMouseButtonUpDelegate.Execute(x, y);
    }
}

void CInputComponent::OnRightMouseButtonDown(int x, int y)
{
    if (OnRightMouseButtonDownDelegate.IsBound())
    {
        OnRightMouseButtonDownDelegate.Execute(x, y);
    }
}

void CInputComponent::OnRightMouseButtonUp(int x, int y)
{
    if (OnRightMouseButtonUpDelegate.IsBound())
    {
        OnRightMouseButtonUpDelegate.Execute(x, y);
    }
}

void CInputComponent::OnMouseMove(int x, int y)
{
    if (OnMouseMoveDelegate.IsBound())
    {
        OnMouseMoveDelegate.Execute(x, y);
    }
}

void CInputComponent::OnMouseWheel(int x, int y, float inDelta)
{
    if (OnMouseWheelDelegate.IsBound())
    {
        OnMouseWheelDelegate.Execute(x, y, inDelta);
    }
}

