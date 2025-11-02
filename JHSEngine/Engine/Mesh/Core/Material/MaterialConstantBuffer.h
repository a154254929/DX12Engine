#pragma once
#include "../../../Math/EngineMath.h"

class FMaterialConstantBuffer
{
public:
	FMaterialConstantBuffer();

	UINT32 materialType;
	UINT32 xx1;
	UINT32 xx2;
	UINT32 xx3;

	XMFLOAT4 baseColor;
	float roughness;
	XMFLOAT4X4 transformation;
};