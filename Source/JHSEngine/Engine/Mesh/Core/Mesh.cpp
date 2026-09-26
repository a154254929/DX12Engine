#include "Mesh.h"

#include "../../Component/Mesh/Core/ShellMeshComponent.h"
#include "../../Config/EngineRenderConfig.h"

GMesh::GMesh()
    :GActorObject()
{
    FCreateObjectParam param;
    param.owner = this;
    meshComponent = CreateObject<CShellMeshComponent>(param, new CShellMeshComponent());
}

void GMesh::Init()
{
    if (meshComponent)
    {
        meshComponent->Init();
    }
}

void GMesh::PreDraw(float deltaTime)
{
}

void GMesh::Draw(float deltaTime)
{
}

void GMesh::PostDraw(float deltaTime)
{
}

void GMesh::SetPickup(bool inPickup)
{
    GetMeshComponent()->SetPickup(inPickup);
}

UINT GMesh::GetMaterialNum()const
{
    return meshComponent->GetMaterialNum();
}

vector<CMaterial*>* GMesh::GetMaterials()
{
    return meshComponent->GetMaterials();
}

void GMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
    meshComponent = InMeshComponent;
}

void GMesh::SetRenderLayerType(EMeshRenderLayerType inMeshRenderLayerType)
{
    meshComponent->SetRenderLayerType(inMeshRenderLayerType);
}

void GMesh::SetCastShadows(bool inCastShadows)
{
    if (meshComponent)
    {
        meshComponent->SetCastShadow(inCastShadows);
    }
}

bool GMesh::IsCastShadow() const
{
    if (meshComponent)
    {
        return meshComponent->IsCastShadow();
    }
    return false;
}

void GMesh::SetPosition(const XMFLOAT3& inPosition)
{
    Super::SetPosition(inPosition);
    if (meshComponent)
    {
        meshComponent->SetPosition(inPosition);
    }
}

void GMesh::SetRotation(const fvector_3d& inRotation)
{
    Super::SetRotation(inRotation);
    if (meshComponent)
    {
        meshComponent->SetRotation(inRotation);
    }
}

void GMesh::SetScale(const fvector_3d& inScale)
{
    Super::SetScale(inScale);
    if (meshComponent)
    {
        meshComponent->SetScale(inScale);
    }
}
