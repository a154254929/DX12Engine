#pragma once
#include "../EngineMinimal.h"

namespace EngineMath
{
    enum ECubeMapFace
    {
        PositiveX  = 0, 
        NegativeX = 1,
        PositiveY = 2,
        NegativeY = 3,
        PositiveZ = 4,
        NegativeZ = 5,
        Invalid = 6
    };
    
    XMFLOAT4X4 IdentityMatrix4x4();
    
    XMFLOAT3 ToFloat3(const fvector_3d& inV3d);
    
    fvector_3d ToVector3d(const XMFLOAT3& inV3d);
    
    fvector_3d GetPointSphericalCoordinates(const fvector_3d& inV3d);
    
    ECubeMapFace GetSampleCubemapIndex(fvector_3d inPointPosition);
}