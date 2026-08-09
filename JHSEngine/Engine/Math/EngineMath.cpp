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

    fvector_3d GetPointSphericalCoordinates(const fvector_3d& inV3d)
    {
        float radius = fvector_3d::dot(inV3d, inV3d);
        radius = sqrt(radius);
        float theta = acos(inV3d.z / radius);
        float phi = atan2f(inV3d.y, inV3d.x);
        return fvector_3d(radius, theta, phi);
    }
}
