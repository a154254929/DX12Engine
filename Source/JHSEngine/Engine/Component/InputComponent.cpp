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
        if (GetAsyncKeyState('W') & KF_UP)
        {
            inputKey.keyName = "W";
        }
        else if (GetAsyncKeyState('S') & KF_UP)
        {
            inputKey.keyName = "S";
        }
        else if (GetAsyncKeyState('A') & KF_UP)
        {
            inputKey.keyName = "A";
        }
        else if (GetAsyncKeyState('D') & KF_UP)
        {
            inputKey.keyName = "D";
        }
        else if (GetAsyncKeyState('E') & KF_UP)
        {
            inputKey.keyName = "E";
        }
        else if (GetAsyncKeyState('R') & KF_UP)
        {
            inputKey.keyName = "R";
        }
        else if (GetAsyncKeyState('Q') & KF_UP)
        {
            inputKey.keyName = "Q";
        }
        else if (GetAsyncKeyState('T') & KF_UP)
        {
            inputKey.keyName = "T";
        }
        else if (GetAsyncKeyState('Y') & KF_UP)
        {
            inputKey.keyName = "Y";
        }
        else if (GetAsyncKeyState('F') & KF_UP)
        {
            inputKey.keyName = "F";
        }
        else if (GetAsyncKeyState(VK_MENU) & KF_UP)
        {
            inputKey.keyName = "alt";
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

