#pragma once
#include "CoreObject/CoreMinimalObject.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	CCamera* GetCamera() { return camera; };

protected:
	CVARIABLE()
	CCamera* camera;

};
