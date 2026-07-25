#include "EngineMath.h"

namespace EngineMath
{
    XMFLOAT4X4 IdentityMatrix4x4()
    {
        return XMFLOAT4X4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    XMFLOAT3 ToFloat3(const fvector_3d& inV3d)
    {
        return XMFLOAT3(inV3d.x, inV3d.y, inV3d.z);
    }

    fvector_3d ToVector3d(const XMFLOAT3& inFloat3d)
    {
        return fvector_3d(inFloat3d.x, inFloat3d.y, inFloat3d.z);
    }
}
