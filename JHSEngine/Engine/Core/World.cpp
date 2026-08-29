#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
    camera = CreateObject<GCamera>(new GCamera());
}

bool CWorld::LineTraceBySingle(FCollisionResult& outHitResult, fvector_3d inStart, fvector_3d inEnd)
{
    
    return false;
}
