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

	CVARIABLE()
	CCamera* camera;

};
