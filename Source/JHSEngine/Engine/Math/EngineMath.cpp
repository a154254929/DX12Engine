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

    fmatrix_4x4 ToMatrix4x4(const XMFLOAT4X4& inFloat4x4)
    {
        return fmatrix_4x4(
            inFloat4x4._11, inFloat4x4._12, inFloat4x4._13, inFloat4x4._14,
            inFloat4x4._21, inFloat4x4._22, inFloat4x4._23, inFloat4x4._24,
            inFloat4x4._31, inFloat4x4._32, inFloat4x4._33, inFloat4x4._34,
            inFloat4x4._41, inFloat4x4._42, inFloat4x4._43, inFloat4x4._44
        );
    }

    XMFLOAT4X4 ToFloat4x4(const fmatrix_4x4& inMatrix4x4)
    {
        return XMFLOAT4X4(
            inMatrix4x4.m11, inMatrix4x4.m12, inMatrix4x4.m13, inMatrix4x4.m14,
            inMatrix4x4.m21, inMatrix4x4.m22, inMatrix4x4.m23, inMatrix4x4.m24,
            inMatrix4x4.m31, inMatrix4x4.m32, inMatrix4x4.m33, inMatrix4x4.m34,
            inMatrix4x4.m41, inMatrix4x4.m42, inMatrix4x4.m43, inMatrix4x4.m44
        );
    }

    fmatrix_3x3 ToMatrix3x3(const XMFLOAT3X3& inMatrix3x3)
    {
        return fmatrix_3x3(
            inMatrix3x3._11, inMatrix3x3._12, inMatrix3x3._13,
            inMatrix3x3._21, inMatrix3x3._22, inMatrix3x3._23,
            inMatrix3x3._31, inMatrix3x3._32, inMatrix3x3._33
        );
    }

    XMFLOAT3X3 ToFloat3x3(const fmatrix_3x3& inMatrix3x3)
    {
        return XMFLOAT3X3(
            inMatrix3x3.m11, inMatrix3x3.m12, inMatrix3x3.m13,
            inMatrix3x3.m21, inMatrix3x3.m22, inMatrix3x3.m23,
            inMatrix3x3.m31, inMatrix3x3.m32, inMatrix3x3.m33
        );
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
    
    ECubeMapFace GetSampleCubemapIndex(const fvector_3d& inPointPosition)
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
    
    bool IsRange(float inValue, float inMin, float inMax)
    {
        return inValue >= inMin && inValue <= inMax;
    }

    int GetSample8CubeIndex(const fvector_3d& inRelativePosition)
    {
        fvector_3d sphericalCoords = GetPointSphericalCoordinates(inRelativePosition);
        float theta = sphericalCoords.y;
        float phi = sphericalCoords.z;
        
        if (IsRange(theta, 0.f, XM_PIDIV2))
        {
            if (IsRange(phi, 0, XM_PIDIV2))
            {
                return 0;
            }
            else if (IsRange(phi, XM_PIDIV2, XM_PI))
            {
                return 1;
            }
            else if (IsRange(phi, -XM_PI, -XM_PIDIV2))
            {
                return 2;
            }
            else if (IsRange(phi, -XM_PIDIV2, 0.f))
            {
                return 3;
            }
        }
        else if (IsRange(theta, XM_PIDIV2, XM_PI))
        {
            if (IsRange(phi, 0, XM_PIDIV2))
            {
                return 4;
            }
            else if (IsRange(phi, XM_PIDIV2, XM_PI))
            {
                return 5;
            }
            else if (IsRange(phi, -XM_PI, -XM_PIDIV2))
            {
                return 6;
            }
            else if (IsRange(phi, -XM_PIDIV2, 0.f))
            {
                return 7;
            }
        }
        
        return -1;
    }

    void BuildMatrixFromPositionRotationScale(
        XMFLOAT4X4& outMatrix,
        XMFLOAT3 inPosition,
        fvector_3d inScale,
        XMFLOAT3 inRightVector,
        XMFLOAT3 inUpVector,
        XMFLOAT3 inForwardVector
    )
    {
        outMatrix = {
            inRightVector.x * inScale.x,		inUpVector.x * inScale.y,	inForwardVector.x * inScale.z,	0.f,
            inRightVector.y * inScale.x,		inUpVector.y * inScale.y,	inForwardVector.y * inScale.z,	0.f,
            inRightVector.z * inScale.x,		inUpVector.z * inScale.y,	inForwardVector.z * inScale.z,	0.f,
            inPosition.x,					    inPosition.y,				inPosition.z,					1.f
        };
        
    }

    void BuildMatrixInvFromPositionRotationScale(
        XMMATRIX& outInvMatrix, XMFLOAT3 inPosition, fvector_3d inScale,
        XMFLOAT3 inRightVector, XMFLOAT3 inUpVector, XMFLOAT3 inForwardVector)
    {XMFLOAT4X4 rotateArrowMatrix;
        BuildMatrixFromPositionRotationScale(
            rotateArrowMatrix,
            inPosition,
            inScale,
            inRightVector,
            inUpVector,
            inForwardVector
        );
    
        XMMATRIX worldMatrix = XMLoadFloat4x4(&rotateArrowMatrix);
        XMVECTOR worldMatrixRIXDeterminant = XMMatrixDeterminant(worldMatrix);
        outInvMatrix = XMMatrixInverse(&worldMatrixRIXDeterminant, worldMatrix);
    }

    fquat BuildQuat(const fvector_3d inForwardVector, const fvector_3d inUpVector)
    {
        fvector_3d rightVector = fvector_3d::cross_product(inUpVector, inForwardVector);
        rightVector.normalize();
        fvector_3d upVector = fvector_3d::cross_product(inForwardVector, rightVector);
        fmatrix_3x3 rotationMatrix = {
            rightVector.x,		upVector.x,	    inForwardVector.x,
            rightVector.y,		upVector.y,     inForwardVector.y,
            rightVector.z,		upVector.z,	    inForwardVector.z
        };
        return math_utils::matrix_to_quat(rotationMatrix);
    }

    void BuildMatrixWithRotation(fmatrix_3x3& outMatrix, XMFLOAT3 inRightVector, XMFLOAT3 inUpVector,
                                 XMFLOAT3 inForwardVector)
    {
        outMatrix = {
            inRightVector.x,		inUpVector.x,	inForwardVector.x,
            inRightVector.y,		inUpVector.y,	inForwardVector.y,
            inRightVector.z,		inUpVector.z,	inForwardVector.z
        };
    }
}
