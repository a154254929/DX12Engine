#pragma once
#include "../EngineMinimal.h"

namespace EngineMath
{
    enum ECubeMapFace
    {
        POSITIVE_X  = 0, 
        NEGATIVE_X = 1,
        POSITIVE_Y = 2,
        NEGATIVE_Y = 3,
        POSITIVE_Z = 4,
        NEGATIVE_Z = 5,
        Invalid = 6
    };
    
    XMFLOAT4X4 IdentityMatrix4x4();
    
    XMFLOAT3 ToFloat3(const fvector_3d& inV3d);
    
    fvector_3d ToVector3d(const XMFLOAT3& inV3d);
    
    fmatrix_4x4 ToMatrix4x4(const XMFLOAT4X4& inFloat4x4);
    XMFLOAT4X4 ToFloat4x4(const fmatrix_4x4& inMatrix4x4);
    
    fmatrix_3x3 ToMatrix3x3(const XMFLOAT3X3& inMatrix3x3);
    XMFLOAT3X3 ToFloat3x3(const fmatrix_3x3& inMatrix3x3);
    
    bool IsAngleRange(float inAngle, float x, float y);
    
    bool IsAngleAxisRange(float inAngle, float x, float y, float inCriticalValue, bool bCompMin);
    
    bool IsPointInCubeMapViewport(
        float inPointTheta
        , float inPointPhi
        , fvector_4d inCompareValue
        , int bAxisRange
        , float inCriticalValue = 0.f
        , bool bCompMin = false
    );
    
    fvector_3d GetPointSphericalCoordinates(const fvector_3d& inV3d);
    
    ECubeMapFace GetSampleCubemapIndex(const fvector_3d& inPointPosition);
    
    int GetSample8CubeIndex(const fvector_3d& inPointPosition);
    
    void BuildMatrixFromPositionRotationScale(
        XMFLOAT4X4& outMatrix,
        XMFLOAT3 inPosition,
        fvector_3d inScale,
        XMFLOAT3 inRightVector,
        XMFLOAT3 inUpVector,
        XMFLOAT3 inForwardVector
    );
    
    void BuildMatrixInvFromPositionRotationScale(
        XMMATRIX& outInvMatrix,
        XMFLOAT3 inPosition,
        fvector_3d inScale,
        XMFLOAT3 inRightVector,
        XMFLOAT3 inUpVector,
        XMFLOAT3 inForwardVector
    );
    
    fquat BuildQuat(
        const fvector_3d inForwardVector,
        const fvector_3d inUpVector = fvector_3d(0.f, 1.f, 0.f)
    );
    
    void BuildMatrixWithRotation(
        fmatrix_3x3& outMatrix,
        XMFLOAT3 inRightVector,
        XMFLOAT3 inUpVector,
        XMFLOAT3 inForwardVector
    );
    
    template<typename T>
    T Lerp(const T& a, const T& b, float t)
    {
        return a + (b - a) * t;
    }
}
