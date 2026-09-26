#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

class CWorld;

struct FRaycastSystemLibrary
{
    static bool HitResultByScreen(CWorld* world, int screenX, int screenY, FCollisionResult& outHitResult);
    
    static bool HitSpecialObjectsResultByScreen(CWorld* world, GActorObject* inSpecialObject, int screenX, int screenY, FCollisionResult& outHitResult);
};
