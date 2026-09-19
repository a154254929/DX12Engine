#pragma once
#include "EngineMinimal.h"
#include "Rendering/Core/DirectX/RenderingPipeline/Geometry/RenderingData.h"

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
    
public:
    std::weak_ptr<FRenderingData> renderingData;
};
