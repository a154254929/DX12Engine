#pragma once
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInforDelegate, void, const FInputKey &);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int, float);

class CInputComponent : public CComponent
{
public:
    CVARIABLE()
    FCaptureKeyboardInforDelegate captureKeyboardInforDelegate;

    CVARIABLE()
    FCaptureOnMouseDelegate OnLeftMouseButtonDownDelegate;

    CVARIABLE()
    FCaptureOnMouseDelegate OnLeftMouseButtonUpDelegate;

    CVARIABLE()
    FCaptureOnMouseDelegate OnRightMouseButtonDownDelegate;

    CVARIABLE()
    FCaptureOnMouseDelegate OnRightMouseButtonUpDelegate;

    CVARIABLE()
    FCaptureOnMouseDelegate OnMouseMoveDelegate;

    CVARIABLE()
    FCaptureOnMouseWheelDelegate OnMouseWheelDelegate;

public:
    virtual void BeginInit();
    virtual void Tick(float deltaTime);

    virtual void OnLeftMouseButtonDown(int x, int y);
    virtual void OnLeftMouseButtonUp(int x, int y);

    virtual void OnRightMouseButtonDown(int x, int y);
    virtual void OnRightMouseButtonUp(int x, int y);
    
    virtual void OnMouseMove(int x, int y);
    virtual void OnMouseWheel(int x, int y, float inDelta);
};