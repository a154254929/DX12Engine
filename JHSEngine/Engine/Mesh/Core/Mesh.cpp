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

void GMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
	if (meshComponent)
	{
		meshComponent->BuildMesh(inRenderingData);
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
