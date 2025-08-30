#pragma once

#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();
	void SetPosition(const XMFLOAT3& inPosition);
	void SetForwardVector(const XMFLOAT3& inForwardVector);
	void SetRightVector(const XMFLOAT3& inRightVector);
	void SetUpVector(const XMFLOAT3& inUpVector);

	XMFLOAT3& GetPosition() { return position; };
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

	XMFLOAT3 forwardVector;
	XMFLOAT3 rightVector;
	XMFLOAT3 upVector;
};