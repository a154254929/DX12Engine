#pragma once
#include "EngineMinimal.h"

class CComponent;
class GActorObject;

struct FCollisionResult
{
    FCollisionResult();
    
    bool bIsCollided;
    float collisionDistance;
    float collisionTime;
    fvector_3d collisionPoint;
    fvector_3d collisionNormal;
    
    CComponent* collisionComponent;
    GActorObject* collisionActor;
};
