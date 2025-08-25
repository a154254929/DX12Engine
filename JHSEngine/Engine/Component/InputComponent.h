#pragma once
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInforDelegate, void, const FInputKey &);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);

class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInforDelegate captureKeyboardInforDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseButtonDownDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseButtonUpDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseMoveDelegate;

public:
	virtual void BeginInit();
	virtual void Tick(float deltaTime);

	virtual void OnMouseButtonDown(int x, int y);
	virtual void OnMouseButtonUp(int x, int y);
	virtual void OnMouseMove(int x, int y);
};