#pragma once
#include "CoreObject/CoreMinimalObject.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	GCamera* GetCamera() { return camera; };

protected:
	CVARIABLE()
	GCamera* camera;

};
