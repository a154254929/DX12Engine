#include "RaycastSystemLibrary.h"
#include "../Core/Camera.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/World.h"
#include "../Collision/CollisionSceneQuery.h"


bool GetRaycastParam(
    CWorld* world,
    fvector_2d screenPosition,
    XMVECTOR& originPoint,
    XMVECTOR& direction,
    XMMATRIX& viewInvMatrix
)
{
    if (GCamera* camera = world->GetCamera())
    {
        XMFLOAT4X4 projectMatrix = camera->projectMatrix;

        //projectMatrix
        fvector_2d screenInfo = fvector_2d(FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
        fvector_2d ndcPos = fvector_2d(screenPosition.x / screenInfo.x * 2.0f - 1.0f, 1.0f - screenPosition.y / screenInfo.y * 2.0f);
        fvector_2d viewPos = fvector_2d(ndcPos.x / projectMatrix._11, ndcPos.y / projectMatrix._22);

        originPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        direction = XMVectorSet(viewPos.x, viewPos.y, 1.0f, 1.0f);
    
        XMMATRIX viewMatrix = XMLoadFloat4x4(&camera->viewMatrix);
        XMVECTOR viewMatrixDeterminant = XMMatrixDeterminant(viewMatrix);
        viewInvMatrix = XMMatrixInverse(&viewMatrixDeterminant, viewMatrix);
        return true;
    }
    return false;
}


bool FRaycastSystemLibrary::HitResultByScreen(CWorld* world, int screenX, int screenY, FCollisionResult& outResult)
{
    XMVECTOR viewOrigin;
    XMVECTOR viewDir;
    XMMATRIX viewInvMatrix;
    if (GetRaycastParam(world, fvector_2d(screenX, screenY), viewOrigin, viewDir, viewInvMatrix))
    {
    
        return FCollisionSceneQuery::RaySingle(viewOrigin, viewDir, viewInvMatrix, outResult);
    }
    return false;
}

bool FRaycastSystemLibrary::HitSpecialObjectsResultByScreen(
    CWorld* world,
    GActorObject* inSpecialObject,
    int screenX, int screenY,
    FCollisionResult& outHitResult)
{
    XMVECTOR viewOrigin;
    XMVECTOR viewDir;
    XMMATRIX viewInvMatrix;
    if (GetRaycastParam(world, fvector_2d(screenX, screenY), viewOrigin, viewDir, viewInvMatrix))
    {
    
        FCollisionSceneQuery::RaySingle(inSpecialObject, viewOrigin, viewDir, viewInvMatrix, outHitResult);
    }
    return false;
}