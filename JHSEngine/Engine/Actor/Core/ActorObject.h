#pragma once
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;
//G->Game
class GActorObject : public CCoreMinimalObject
{
	CVARIABLE()
	CTransformationComponent* transformationComponent;

public:
	GActorObject();

	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return transformationComponent; };

public:
	void SetPosition(const XMFLOAT3& inPosition);
	void SetRotation(const fvector_3d& inRotation);
	void SetScale(const fvector_3d& inScale);

	XMFLOAT3& GetPosition();
	fvector_3d GetRotation() const;
	fvector_3d GetScale() const;

	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();
};