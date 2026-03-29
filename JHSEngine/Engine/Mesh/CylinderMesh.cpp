#include "CylinderMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GCylinderMesh::Init()
{
    Super::Init();
}

void GCylinderMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GCylinderMesh::CreateMesh(float inTopRadius, float inBottomRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    SetMeshComponent(GetMeshManager()->CreateCylinderMeshComponent(inTopRadius, inBottomRadius, inHeight, inAxialSubdivision, inHeightSubdivision));
}