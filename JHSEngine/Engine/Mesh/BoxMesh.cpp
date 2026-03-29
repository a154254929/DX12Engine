#include "BoxMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GBoxMesh::Init()
{
    Super::Init();
}

void GBoxMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GBoxMesh::CreateMesh(float inHeight, float inWidth, float inDepth)
{
    SetMeshComponent(GetMeshManager()->CreateBoxMeshComponent(inHeight, inWidth, inDepth));
}