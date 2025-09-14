#pragma once
#include "Viewport/Viewport.h"
#include "../Interface/DirectXDeviceInterface.h"
#include "../Actor/Core/ActorObject.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
enum ECameraType;
class GCamera
	: public GActorObject
	, public FViewport
	, public IDirectXDeviceInterface
{

	CVARIABLE()
	CInputComponent* inputComponent;
public:
	GCamera();

	virtual void BeginInit();
	virtual void Tick(float deltaTime);

	virtual void ExecuteKeboard(const FInputKey& inputKey);

	virtual void BuildViewMatrix(float deltaTime);

public:
	virtual void OnMouseButtonDown(int x, int y);
	virtual void OnMouseButtonUp(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseWheel(int x, int y, float inDelta);

	virtual void MoveForward(float inValue);
	virtual void MoveRight(float inValue);
	virtual void MoveUp(float inValue);
protected:
	void RotateAroundYAxis(float inRotateDegrees);
	void RotateAroundXAxis(float inRotateDegrees);
public:

	FORCEINLINE CInputComponent* GetInputComponent() { return inputComponent; }

protected:
	POINT lsatMousePosition;

	bool bLeftMosueDown{ false };

	//鼠标灵敏度
	float mounseSensitivity;

	ECameraType cameraType;

	//球面坐标
	float radius;
	float theta;
	float phi;
};