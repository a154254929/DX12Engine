#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
#include "Material/Material.h"

GMesh::GMesh()
	:GActorObject()
{
	materials.push_back(CreateObject<CMaterial>(new CMaterial()));
}

void GMesh::Init()
{
}

void GMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
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

UINT GMesh::GetMaterialNum() const
{
	return materials.size();
}
