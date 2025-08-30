#pragma once
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"

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

	virtual void BuildViewMatrix(float deltaTime);

public:
	virtual void OnMouseButtonDown(int x, int y);
	virtual void OnMouseButtonUp(int x, int y);
	virtual void OnMouseMove(int x, int y);

	virtual void MoveForward(float inValue);
	virtual void MoveRight(float inValue);
public:

	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return transformationComponent; }

	FORCEINLINE CInputComponent* GetInputComponent() { return inputComponent; }

protected:
	POINT lsatMousePosition;
};