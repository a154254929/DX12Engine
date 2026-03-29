#include "AesmaMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GAesmaMesh::Init()
{
    Super::Init();
}

void GAesmaMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GAesmaMesh::CreateMesh(
    float inOuterRadius,
    float inInnerRadius,
    float inHeight,
    uint32_t inAxialSubdivision,
    uint32_t inHeightSubdivision
)
{
    SetMeshComponent(GetMeshManager()->CreateAesmaMeshComponent(inOuterRadius, inInnerRadius, inHeight, inAxialSubdivision, inHeightSubdivision));
}