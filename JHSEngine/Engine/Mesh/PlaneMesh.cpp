#include "PlaneMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GPlaneMesh::Init()
{
    Super::Init();
}

void GPlaneMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GPlaneMesh::CreateMesh(float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
{
    SetMeshComponent(GetMeshManager()->CreatePlaneMeshComponent(inHeight, inWidth, inHeightSubdivide, inWidthSubdivide));
}