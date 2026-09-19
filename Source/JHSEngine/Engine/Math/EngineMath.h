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
    
    ECubeMapFace GetSampleCubemapIndex(fvector_3d inPointPosition);
}