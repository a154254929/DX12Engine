#pragma once
#include "EngineMinimal.h"

class CComponent;
class GActor;

struct FCollisionResult
{
    FCollisionResult();
    
    bool bIsCollided;
    float collisionDistance;
    float collisionTime;
    fvector_3d collisionPoint;
    fvector_3d collisionNormal;
    
    CComponent* collisionComponent;
    GActor* collisionActor;
};
