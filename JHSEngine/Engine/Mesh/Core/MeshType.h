#pragma once
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex
{
	FVertex(const XMFLOAT3 &inPos, const XMFLOAT4 &inColor);

	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct FMeshRenderingData
{
	vector<FVertex> vertexData;
	vector<uint16_t> indexData;
};