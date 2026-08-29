#include "RaycastSystemLibrary.h"
#include "../Core/Camera.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/World.h"

bool FRaycastSystemLibrary::HitResultByScreen(CWorld* world, int screenX, int screenY, FCollisionResult)
{
    if (GCamera* camera = world->GetCamera())
    {
        XMFLOAT4X4 projectMatrix = camera->projectMatrix;
    
        //projectMatrix
        fvector_2d screenInfo = fvector_2d(FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
        fvector_2d screenPos = fvector_2d(screenX, screenY);
        fvector_2d ndcPos = fvector_2d(screenPos.x / screenInfo.x * 2.0f - 1.0f, 1.0f - screenPos.y / screenInfo.y * 2.0f);
        fvector_2d viewPos = fvector_2d(ndcPos.x / projectMatrix._11, ndcPos.y / projectMatrix._22);
    
        XMVECTOR viewOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        XMVECTOR viewDir = XMVectorSet(viewPos.x, viewPos.y, 1.0f, 1.0f);
        
        XMMATRIX viewMatrix = XMLoadFloat4x4(&camera->viewMatrix);
        XMMATRIX viewInvMatrix = XMMatrixInverse(&XMMatrixDeterminant(viewMatrix), viewMatrix);
    }
}
