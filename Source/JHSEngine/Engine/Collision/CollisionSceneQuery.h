#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

struct FCollisionSceneQuery
{
    static bool RaySingle(
        const XMVECTOR& originPoint
        , const XMVECTOR& direction
        , const XMMATRIX& viewInvMatrix
        , FCollisionResult& outResult
    );
    
    static bool RaySingle(
        GActorObject* inSpecialObject,
        const XMVECTOR& originPoint
        , const XMVECTOR& direction
        , const XMMATRIX& viewInvMatrix
        , FCollisionResult& outResult
    );
};