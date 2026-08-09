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

    struct FCubemapAxialRangeR
    {
        FCubemapAxialRangeR()
            : PositiveX(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, XM_PIDIV4, XM_PIDIV4* 3))
            , NegativeX(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, XM_PIDIV4 * 5, -XM_PIDIV4 * 7))
            , PositiveY(fvector_4d(0, XM_PIDIV4, 0, XM_PI * 2))
            , NegativeY(fvector_4d(XM_PIDIV4 * 3, XM_PI, 0, XM_PI * 2))
            , PositiveZ(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, -XM_PIDIV4, XM_PIDIV4))
            , NegativeZ(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, XM_PIDIV4 * 3, XM_PIDIV4 * 5))
        {
            
        }
        const fvector_4d PositiveX;
        const fvector_4d NegativeX;
        
        const fvector_4d PositiveY;
        const fvector_4d NegativeY;
        
        const fvector_4d PositiveZ;
        const fvector_4d NegativeZ;
    };
    
    static FCubemapAxialRangeR cubemapAxialRangeR;
    
    ECubeMapFace GetSampleCubemapIndex(fvector_3d inPointPosition)
    {
        fvector_3d sphericalCoords = GetPointSphericalCoordinates(inPointPosition);
        float theta = sphericalCoords.y;
        float phi = sphericalCoords.z;

        
        
        
        return Invalid;
    }
}
