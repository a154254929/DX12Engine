#pragma once
#include "Viewport.h"
#include "Object/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport
{
	CVARIABLE()
	CTransformationComponent* transformationComponent;

	CVARIABLE()
	CInputComponent* inputComponent;
public:
};