#pragma once
#include "Viewport.h"
#include "Object/CoreMinimalObject.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport
{
	CVARIABLE()
	CTransformationComponent* transformationComponent;

	CVARIABLE()
	CInputComponent* inputComponent;
public:
	CCamera();

	virtual void BeginInit();
	virtual void Tick(float deltaTime);

	virtual void ExecuteKeboard(const FInputKey& inputKey);
public:

	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return transformationComponent; }

	FORCEINLINE CInputComponent* GetInputComponent() { return inputComponent; }
};