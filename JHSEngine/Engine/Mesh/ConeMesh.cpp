#include "ConeMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GConeMesh::Init()
{
    Super::Init();
}

void GConeMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GConeMesh::CreateMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    SetMeshComponent(GetMeshManager()->CreateConeMeshComponent(inRadius, inHeight, inAxialSubdivision, inHeightSubdivision));
}