#pragma once
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex
{
	FVertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor);
	FVertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor, const XMFLOAT3& inNormal);

	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
	XMFLOAT3 utangent;
	XMFLOAT3 vtangent;
};

struct FMeshRenderingData
{
	vector<FVertex> vertexData;
	vector<uint16_t> indexData;

public:
	UINT GetVertexSizeInBytes() { return vertexData.size() * sizeof(FVertex); }
	UINT GetIndexSizeInBytes() { return indexData.size() * sizeof(uint16_t); }
};