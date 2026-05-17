#include "SphereMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GSphereMesh::Init()
{
    Super::Init();
}

void GSphereMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GSphereMesh::CreateMesh(float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision, bool bReverse)
{
    SetMeshComponent(GetMeshManager()->CreateSphereMeshComponent(inRadius, inAxialSubdivision, inHeightSubdivision, bReverse));
}