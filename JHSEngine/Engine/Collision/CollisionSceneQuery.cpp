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
        
        XMVECTOR objectOriginPoint = XMVector3Transform(originPoint, viewToObjectMatrix);
        XMVECTOR objectDirection = XMVector3TransformNormal(direction, viewToObjectMatrix);
        
        float time = 0.f;
        
        if (renderingData->boundingBox.Intersects(objectOriginPoint, objectDirection, time))
        {
            if (renderingData->meshRenderingData)
            {
                UINT triangleCount =  renderingData->vertexSize / 3;
                for (UINT j = 0; j < triangleCount; ++j)
                {
                    fvector_3d indices;
                    indices.x = renderingData->meshRenderingData->indexData[renderingData->indexOffsetPosition + j * 3 + 0];
                    indices.y = renderingData->meshRenderingData->indexData[renderingData->indexOffsetPosition + j * 3 + 1];
                    indices.z = renderingData->meshRenderingData->indexData[renderingData->indexOffsetPosition + j * 3 + 2];
                    
                    float triangleTestTime = 0.f;
                    
                    XMVECTOR vertexes[3];
                    vertexes[0] = XMLoadFloat3(&renderingData->meshRenderingData->vertexData[renderingData->vertexOffsetPosition + indices.x].position);
                    vertexes[1] = XMLoadFloat3(&renderingData->meshRenderingData->vertexData[renderingData->vertexOffsetPosition + indices.y].position);
                    vertexes[2] = XMLoadFloat3(&renderingData->meshRenderingData->vertexData[renderingData->vertexOffsetPosition + indices.z].position);
                
                    if (TriangleTests::Intersects(objectOriginPoint, objectDirection, vertexes[0], vertexes[1], vertexes[2], triangleTestTime))
                    {
                        
                    }
                
                }
            }
            outResult.collisionTime = time;
            return true;
        }
    }
    return false;
}
