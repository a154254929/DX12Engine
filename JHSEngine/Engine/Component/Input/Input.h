#pragma once
#include "../../Core/Engine.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int, int);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelDelegate, void, int, int, float);

extern FCaptureOnMousesDelegate LeftMouseDownDelegate;
extern FCaptureOnMousesDelegate LeftMouseUpDelegate;
extern FCaptureOnMousesDelegate RightMouseDownDelegate;
extern FCaptureOnMousesDelegate RightMouseUpDelegate;

extern FCaptureOnMousesWheelDelegate MouseWheelDelegate;

extern FCaptureOnMousesDelegate MouseMoveDelegate;