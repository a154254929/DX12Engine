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
        
        XMMATRIX worldMatrix = XMLoadFloat4x4(&renderingData->worldMatrix);
        XMVECTOR worldMatrixDeterminant = XMMatrixDeterminant(worldMatrix);
        XMMATRIX worldToObjectMatrix = XMMatrixInverse(&worldMatrixDeterminant, worldMatrix);
        
       XMMATRIX viewToObjectMatrix = XMMatrixMultiply(viewInvMatrix, worldToObjectMatrix);
        
        XMVECTOR worldOriginPoint = XMVector3Transform(originPoint, viewToObjectMatrix);
        XMVECTOR worldDirection = XMVector3TransformNormal(direction, viewToObjectMatrix);
        
        float time = 0.f;
        
        if (renderingData->boundingBox.Intersects(worldOriginPoint, worldDirection, time))
        {
            outResult.collisionTime = time;
            return true;
        }
    }
    return false;
}
