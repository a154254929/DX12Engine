#include "MeshType.h"

FVertex::FVertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor)
    : position(inPos)
    , color(inColor)
    , texcoord(XMFLOAT2(0.f, 0.f))
{
}

FVertex::FVertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor, const XMFLOAT3& inNormal, const XMFLOAT2& inTexCoord)
    : position(inPos)
    , color(inColor)
    , normal(inNormal)
    , texcoord(inTexCoord)
{
}
