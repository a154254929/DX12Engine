#include "EngineType.h"

FCollisionResult::FCollisionResult()
    : bIsCollided(false)
    , collisionDistance(0.f)
    , collisionTime(0.f)
    , collisionPoint(0.f, 0.f, 0.f)
    , collisionNormal(0.f, 0.f, 0.f)
    , collisionComponent(nullptr)
    , collisionActor(nullptr)
{
}
