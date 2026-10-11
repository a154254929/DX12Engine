#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

struct FCollisionSceneQuery
{
    static bool RaycastSingle(
        const XMVECTOR& originPoint
        , const XMVECTOR& direction
        , const XMMATRIX& viewInvMatrix
        , FCollisionResult& outResult
    );
    
    static bool IsIgnoreComponent(CComponent* inComponent, const std::vector<CComponent*>& inIgnoreComponents);
    
    static bool RaycastSingle(
        GActorObject* inSpecialObject,
        const std::vector<CComponent*>& inIgnoreComponents,
        const XMVECTOR& originPoint,
        const XMVECTOR& direction,
        const XMMATRIX& viewInvMatrix,
        FCollisionResult& outResult
    );
};