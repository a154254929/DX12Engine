#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 world;

	static XMFLOAT4X4 IdentityMatrix4x4();
};