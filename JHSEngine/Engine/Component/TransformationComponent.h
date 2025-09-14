#pragma once

#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();
	void SetPosition(const XMFLOAT3& inPosition);
	void SetRotation(const fvector_3d& inRotation);
	void SetScale(const fvector_3d& inScale);
	void SetForwardVector(const XMFLOAT3& inForwardVector);
	void SetRightVector(const XMFLOAT3& inRightVector);
	void SetUpVector(const XMFLOAT3& inUpVector);

	XMFLOAT3& GetPosition() { return position; };
	fvector_3d GetRotation() const { return fvector_3d(rotation.x, rotation.y, rotation.z); };
	fvector_3d GetScale() const { return fvector_3d(scale.x, scale.y, scale.z); };
	XMFLOAT3& GetForwardVector() { return forwardVector; };
	XMFLOAT3& GetRightVector() { return rightVector; };
	XMFLOAT3& GetUpVector() { return upVector; };

public:
	//矫正Up Right Forward 返回Pos
	void CorrectionVector();


	fvector_3d GetCorrectionPosition();
	void GetCorrectionPosition(fvector_3d& inPosV3);

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT3 forwardVector;
	XMFLOAT3 rightVector;
	XMFLOAT3 upVector;
};