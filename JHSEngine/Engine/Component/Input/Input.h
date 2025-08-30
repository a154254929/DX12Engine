#pragma once
#include "../../Core/Engine.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int, int);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelDelegate, void, int, int, float);

extern FCaptureOnMousesDelegate MouseDownDelegate;
extern FCaptureOnMousesDelegate MouseUpDelegate;

extern FCaptureOnMousesWheelDelegate MouseWheelDelegate;

extern FCaptureOnMousesDelegate MouseMoveDelegate;