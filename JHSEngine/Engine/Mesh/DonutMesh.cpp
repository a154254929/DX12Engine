#include "DonutMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GDonutMesh::Init()
{
    Super::Init();
}

void GDonutMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GDonutMesh::CreateMesh(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
    SetMeshComponent(GetMeshManager()->CreateDonutMeshComponent(inDonutRadius, inThicknessRadius, inDonutAxialSubdivision, inThicknessAxialSubdivision));
}