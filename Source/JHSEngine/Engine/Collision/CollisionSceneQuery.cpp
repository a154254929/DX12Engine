#include "CollisionSceneQuery.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Core/Camera.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/World.h"
#include "../Component/Mesh/Core/MeshComponent.h"
#include "../Actor/Core/ActorObject.h"

void GetRaycastDataByLocal(
    std::shared_ptr<FRenderingData>& inRenderingData,
    const XMVECTOR& originPoint,
    const XMVECTOR& direction,
    const XMMATRIX& viewInvMatrix,
    XMVECTOR& outObjectOriginPoint,
    XMVECTOR& outObjectDirection
)
{
        
    XMMATRIX worldMatrix = XMLoadFloat4x4(&inRenderingData->worldMatrix);
    XMVECTOR worldMatrixDeterminant = XMMatrixDeterminant(worldMatrix);
    XMMATRIX worldToObjectMatrix = XMMatrixInverse(&worldMatrixDeterminant, worldMatrix);
        
    XMMATRIX viewToObjectMatrix = XMMatrixMultiply(viewInvMatrix, worldToObjectMatrix);
        
    outObjectOriginPoint = XMVector3Transform(originPoint, viewToObjectMatrix);
    outObjectDirection = XMVector3Normalize(XMVector3TransformNormal(direction, viewToObjectMatrix));
}

bool FCollisionSceneQuery::RaySingle(
    const XMVECTOR& originPoint,
    const XMVECTOR& direction,
    const XMMATRIX& viewInvMatrix,
    FCollisionResult& outResult
)
{
    float finalTime = FLT_MAX;
        
    for (int i = 0; i < FGeometry::renderingDataArray.size(); ++i)
    {
        std::shared_ptr<FRenderingData> renderingData = FGeometry::renderingDataArray[i];
        if (!renderingData->meshComp->IsPickup())
        {
            continue;
        }
        
        XMVECTOR objectOriginPoint;
        XMVECTOR objectDirection;
        
        GetRaycastDataByLocal(renderingData, originPoint, direction, viewInvMatrix, objectOriginPoint, objectDirection);
        
        float time = FLT_MAX;
        
        if (renderingData->boundingBox.Intersects(objectOriginPoint, objectDirection, time))
        {
            
            if (time < 0 || finalTime <= time)
            {
                continue;
            }
            
            if (renderingData->meshRenderingData)
            {
                UINT triangleCount =  renderingData->indexSize / 3;
                for (UINT j = 0; j < triangleCount; ++j)
                {
                    fvector_3d indices;
                    indices.x = renderingData->meshRenderingData->indexData[renderingData->indexOffsetPosition + j * 3 + 0];
                    indices.y = renderingData->meshRenderingData->indexData[renderingData->indexOffsetPosition + j * 3 + 1];
                    indices.z = renderingData->meshRenderingData->indexData[renderingData->indexOffsetPosition + j * 3 + 2];
                    
                    float triangleTestTime = FLT_MAX;
                    
                    XMVECTOR vertexes[3];
                    vertexes[0] = XMLoadFloat3(&renderingData->meshRenderingData->vertexData[renderingData->vertexOffsetPosition + indices.x].position);
                    vertexes[1] = XMLoadFloat3(&renderingData->meshRenderingData->vertexData[renderingData->vertexOffsetPosition + indices.y].position);
                    vertexes[2] = XMLoadFloat3(&renderingData->meshRenderingData->vertexData[renderingData->vertexOffsetPosition + indices.z].position);
                
                    if (TriangleTests::Intersects(objectOriginPoint, objectDirection, vertexes[0], vertexes[1], vertexes[2], triangleTestTime))
                    {
                        if (finalTime > triangleTestTime)
                        {
                            finalTime = triangleTestTime;
                            outResult.bIsCollided = true;
                            outResult.collisionTime = finalTime;
                            //outResult.collisionDistance = finalTime;
                            outResult.collisionComponent = renderingData->meshComp;
                            //outResult.collisionPoint = ;
                            outResult.collisionActor = dynamic_cast<GActorObject*>(renderingData->meshComp->GetOwner());
                            outResult.renderingData = renderingData;
                        }
                        
                    }
                
                }
            }
        }
    }
    return outResult.bIsCollided;
}

bool FCollisionSceneQuery::RaySingle(
    GActorObject* inSpecialObject,
    const XMVECTOR& originPoint,
    const XMVECTOR& direction,
    const XMMATRIX& viewInvMatrix,
    FCollisionResult& outResult
)
{
    float finalTime = FLT_MAX;
        
    for (int i = 0; i < FGeometry::renderingDataArray.size(); ++i)
    {
        std::shared_ptr<FRenderingData> renderingData = FGeometry::renderingDataArray[i];
        if (!renderingData->meshComp->IsPickup())
        {
            continue;
        }
        
        XMVECTOR objectOriginPoint;
        XMVECTOR objectDirection;
        
        GetRaycastDataByLocal(renderingData, originPoint, direction, viewInvMatrix, objectOriginPoint, objectDirection);
        
        float time = FLT_MAX;
        
        if (renderingData->boundingBox.Intersects(objectOriginPoint, objectDirection, time))
        {
            if (GActorObject* actorObject = dynamic_cast<GActorObject*>(renderingData->meshComp->GetOwner()))
            {
                if (actorObject == inSpecialObject)
                {
                    outResult.bIsCollided = true;
                    outResult.collisionTime = time;
                    //outResult.collisionDistance = finalTime;
                    outResult.collisionComponent = renderingData->meshComp;
                    //outResult.collisionPoint = ;
                    outResult.collisionActor = dynamic_cast<GActorObject*>(renderingData->meshComp->GetOwner());
                    outResult.renderingData = renderingData;
                }
            }
        }
    }
    return outResult.bIsCollided;
}
