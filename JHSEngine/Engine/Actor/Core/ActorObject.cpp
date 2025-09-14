#include "ActorObject.h"
#include "../../Component/TransformationComponent.h"

GActorObject::GActorObject()
{
	transformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& inPosition)
{
	transformationComponent->SetPosition(inPosition);
}

void GActorObject::SetRotation(const fvector_3d& inRotation)
{
	transformationComponent->SetRotation(inRotation);
}

XMFLOAT3& GActorObject::GetPosition()
{
	return transformationComponent->GetPosition();
}

XMFLOAT3& GActorObject::GetForwardVector()
{
	return transformationComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector()
{
	return transformationComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUpVector()
{
	return transformationComponent->GetUpVector();
}