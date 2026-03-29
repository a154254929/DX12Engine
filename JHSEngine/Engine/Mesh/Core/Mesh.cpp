#include "Mesh.h"

#include "../../Component/Mesh/ShellMeshComponent.h"
#include "../../Config/EngineRenderConfig.h"

GMesh::GMesh()
    :GActorObject()
{
    meshComponent = CreateObject<CShellMeshComponent>(new CShellMeshComponent());
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

void GMesh::SetPosition(const XMFLOAT3& inPosition)
{
    GActorObject::SetPosition(inPosition);
    if (meshComponent)
    {
        meshComponent->SetPosition(inPosition);
    }
}

void GMesh::SetRotation(const fvector_3d& inRotation)
{
    GActorObject::SetRotation(inRotation);
    if (meshComponent)
    {
        meshComponent->SetRotation(inRotation);
    }
}

void GMesh::SetScale(const fvector_3d& inScale)
{
    GActorObject::SetScale(inScale);
    if (meshComponent)
    {
        meshComponent->SetScale(inScale);
    }
}
