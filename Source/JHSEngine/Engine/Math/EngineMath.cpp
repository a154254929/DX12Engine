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

    bool IsAngleRange(float inAngle, float x, float y)
    {
        return inAngle >= min(x, y) && inAngle <= max(x, y);
    }

    bool IsAngleAxisRange(float inAngle, float x, float y, float inCriticalValue, bool bCompMin)
    {
        if (bCompMin)
        {
            if (inAngle > inCriticalValue)
            {
                if (inAngle <= max(x, y))
                {
                    return true;
                }
                
            }
            else if (inAngle < inCriticalValue)
            {
                if (inAngle >= min(x, y))
                {
                    return true;
                }
            }
            else
                    return true;
        }
        else
        {
            if (inAngle > 0.f)
            {
                if (inAngle >= max(x, y) && inAngle <= inCriticalValue)
                {
                    return true;
                }
            }
            else
            {
                if (inAngle <= min(x, y) && inAngle >= -inCriticalValue)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool IsPointInCubeMapViewport(
        float inPointTheta
        , float inPointPhi
        , fvector_4d inCompareValue
        , int bAxisRange
        , float inCriticalValue
        , bool bCompMin
    )
    {
        if (IsAngleRange(inPointTheta, inCompareValue.x, inCompareValue.y))
        {
            if (bAxisRange == 0)
            {
                if (IsAngleAxisRange(
                    inPointPhi
                    , inCompareValue.z
                    , inCompareValue.w
                    , inCriticalValue
                    , bCompMin
                ))
                {
                    return true;
                }
            }
            else if (bAxisRange == 1)
            {
                if (inPointPhi >= inCompareValue.z && inPointPhi <= inCompareValue.w)
                {
                    return true;
                }
            }
            else if (bAxisRange == 2)
            {
                if (inPointPhi <= inCompareValue.z && inPointPhi >= inCompareValue.w)
                    return true;
            }
            else
            {
                if (IsAngleRange(inPointPhi, inCompareValue.z, inCompareValue.w))
                {
                    return true;
                }
            }
        }
        return false;
    }

    fvector_3d GetPointSphericalCoordinates(const fvector_3d& inV3d)
    {
        float radius = fvector_3d::dot(inV3d, inV3d);
        radius = sqrt(radius);
        float theta = acos(inV3d.y / radius);
        float phi = atan2f(inV3d.z, inV3d.x);
        return fvector_3d(radius, theta, phi);
    }

    struct FCubemapAxialRangeR
    {
        FCubemapAxialRangeR()
            : PositiveX(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, XM_PIDIV4, -XM_PIDIV4))
            , NegativeX(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, XM_PIDIV4 * 3, -XM_PIDIV4 * 3))
            , PositiveY(fvector_4d(0, XM_PIDIV4, XM_PI * 2, -XM_PI * 2))
            , NegativeY(fvector_4d(XM_PIDIV4 * 3, XM_PI, XM_PI * 2, -XM_PI * 2))
            , PositiveZ(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, XM_PIDIV4, XM_PIDIV4 * 3))
            , NegativeZ(fvector_4d(XM_PIDIV4, XM_PIDIV4 * 3, -XM_PIDIV4, -XM_PIDIV4 * 3))
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

        if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.PositiveX,3,0.f,true))
        {
            return ECubeMapFace::POSITIVE_X;
        }
        else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.NegativeX,0, XM_PI,false))
        {
            return ECubeMapFace::NEGATIVE_X;
        }
        else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.PositiveY, 3))
        {
            return ECubeMapFace::POSITIVE_Y;
        }
        else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.NegativeY, 3))
        {
            return ECubeMapFace::NEGATIVE_Y;
        }
        else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.PositiveZ,1))
        {
            return ECubeMapFace::POSITIVE_Z;
        }
        else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.NegativeZ, 2))
        {
            return ECubeMapFace::NEGATIVE_Z;
        }
        
        
        return Invalid;
    }
}
