#include "MeshType.h"

FVertex::FVertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor)
	: position(inPos)
	, color(inColor)
{
}

FVertex::FVertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor, const XMFLOAT3& inNormal)
	: position(inPos)
	, color(inColor)
	, normal(inNormal)
{
}
