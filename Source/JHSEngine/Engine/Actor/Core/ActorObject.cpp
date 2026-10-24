#include "ActorObject.h"
#include "../../Component/TransformationComponent.h"
#include "Engine/Component/Mesh/Core/MeshComponent.h"
#include "Engine/Math/EngineMath.h"

GActorObject::GActorObject()
{
    FCreateObjectParam param;
    param.owner = this;
    rootComponent = CreateObject<CTransformationComponent>(param, new CTransformationComponent());
}

BoundingBox GActorObject::GetBoundingBox()
{
    BoundingBox aabb;
    GetBoundingBox(aabb);
    return aabb;
}

void GActorObject::GetBoundingBox(BoundingBox& outBoundingBox)
{
    if (rootComponent)
    {
        std::vector<CComponent*> children = rootComponent->GetChildren();
        fvector_3d maxPoint = fvector_3d(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        fvector_3d minPoint = fvector_3d(FLT_MAX, FLT_MAX, FLT_MAX);
        for (auto child : children)
        {
            if (CMeshComponent* meshComp = dynamic_cast<CMeshComponent*>(child))
            {
                
                BoundingBox meshAABB = meshComp->GetBoundingBox();
                fvector_3d center = EngineMath::ToVector3d(meshAABB.Center);
                fvector_3d extent = EngineMath::ToVector3d(meshAABB.Extents);
                
                fvector_3d meshMaxPoint = center + extent;
                fvector_3d meshMinPoint = center - extent;
                
                maxPoint.x = max(maxPoint.x, meshMaxPoint.x);
                maxPoint.y = max(maxPoint.y, meshMaxPoint.y);
                maxPoint.z = max(maxPoint.z, meshMaxPoint.z);
                
                minPoint.x = min(minPoint.x, meshMinPoint.x);
                minPoint.y = min(minPoint.y, meshMinPoint.y);
                minPoint.z = min(minPoint.z, meshMinPoint.z);
            }
        }
        if (maxPoint.x != -FLT_MAX && minPoint.x != FLT_MAX)
        {
            
            XMFLOAT3 xmMaxPoint = EngineMath::ToFloat3(maxPoint);
            XMFLOAT3 xmMinPoint = EngineMath::ToFloat3(minPoint);
            
            XMVECTOR xmvMaxPoint = XMLoadFloat3(&xmMaxPoint);
            XMVECTOR xmvMinPoint = XMLoadFloat3(&xmMinPoint);
            
            XMStoreFloat3(&outBoundingBox.Center, (xmvMaxPoint + xmvMinPoint) * 0.5f);
            XMStoreFloat3(&outBoundingBox.Extents, (xmvMaxPoint - xmvMinPoint) * 0.5f);
        }
    }
}

void GActorObject::SetPosition(const XMFLOAT3& inPosition)
{
    rootComponent->SetPosition(inPosition);
}

void GActorObject::SetRotation(const fvector_3d& inRotation)
{
    rootComponent->SetRotation(inRotation);
}

void GActorObject::SetRotation(const frotator& inRotator)
{
    rootComponent->SetRotation(inRotator);
}

void GActorObject::SetScale(const fvector_3d& inScale)
{
    rootComponent->SetScale(inScale);
}

void GActorObject::SetPickup(bool inPickup)
{
}

XMFLOAT3& GActorObject::GetPosition()
{
    return rootComponent->GetPosition();
}

fvector_3d GActorObject::GetRotation() const
{
    return rootComponent->GetRotation();
}

fvector_3d GActorObject::GetScale() const
{
    return rootComponent->GetScale();
}

XMFLOAT3& GActorObject::GetForwardVector()
{
    return rootComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector()
{
    return rootComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUpVector()
{
    return rootComponent->GetUpVector();
}