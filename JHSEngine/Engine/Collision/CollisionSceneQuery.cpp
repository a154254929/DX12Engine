#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Core/Camera.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/World.h"

bool FCollisionSceneQuery::RaySingle(const XMVECTOR& originPoint, const XMVECTOR& direction,
    const XMMATRIX& viewInvMatrix, FCollisionResult& outResult)
{
    
        
    for (int i = 0; i < FGeometry::renderingDataArray.size(); ++i)
    {
        std::shared_ptr<FRenderingData> renderingData = FGeometry::renderingDataArray[i];
        
        
        
        float time = 0.f;
        
        if (renderingData->boundingBox.Intersects(originPoint, direction, time))
        {
            outResult.collisionTime = time;
            return true;
        }
    }
    return false;
}
