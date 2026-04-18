#include "LightComponent.h"
#include "../../../Manager/LightManager.h"
#include "../../Mesh/Core/MeshComponent.h"

CLightComponent::CLightComponent()
{
    GetLightManager()->AddLight(this);
}

CLightComponent::~CLightComponent()
{
    GetLightManager()->RemoveLight(this);
}

void CLightComponent::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    lightMesh->SetPosition(inPosition);
}

void CLightComponent::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    lightMesh->SetRotation(inRotation * -1);
}

void CLightComponent::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    lightMesh->SetScale(inScale);
}

void CLightComponent::SetForwardVector(const XMFLOAT3& inForwardVector)
{
    Super::SetForwardVector(inForwardVector);
    lightMesh->SetForwardVector(inForwardVector);
}

void CLightComponent::SetRightVector(const XMFLOAT3& inRightVector)
{
    Super::SetRightVector(inRightVector);
    lightMesh->SetRightVector(inRightVector);
}

void CLightComponent::SetUpVector(const XMFLOAT3& inUpVector)
{
    Super::SetUpVector(inUpVector);
    lightMesh->SetUpVector(inUpVector);
}

void CLightComponent::SetLightMesh(CMeshComponent* inLightMesh)
{
    lightMesh = inLightMesh;
}