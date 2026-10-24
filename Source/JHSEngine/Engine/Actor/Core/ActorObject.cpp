#include "ActorObject.h"
#include "../../Component/TransformationComponent.h"

GActorObject::GActorObject()
{
    FCreateObjectParam param;
    param.owner = this;
    rootComponent = CreateObject<CTransformationComponent>(param, new CTransformationComponent());
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