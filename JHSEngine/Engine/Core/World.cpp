#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
    FCreateObjectParam param;
    param.owner = this;
    camera = CreateObject<GCamera>(param, new GCamera());
}

bool CWorld::LineTraceBySingle(FCollisionResult& outHitResult, fvector_3d inStart, fvector_3d inEnd)
{
    
    return false;
}
