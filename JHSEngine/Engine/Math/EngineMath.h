#pragma once
#include "../EngineMinimal.h"

namespace EngineMath
{
    XMFLOAT4X4 IdentityMatrix4x4();
    
    XMFLOAT3 ToFloat3(const fvector_3d& inV3d);
    
    fvector_3d ToVector3d(const XMFLOAT3& inV3d);
    
    fvector_3d GetPointSphericalCoordinates(const fvector_3d& inV3d);
}