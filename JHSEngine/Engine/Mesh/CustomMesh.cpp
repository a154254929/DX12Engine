#include "CustomMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"

void GCustomMesh::Init()
{
    Super::Init();
}

void GCustomMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GCustomMesh::CreateMesh(string& inPath)
{
    SetMeshComponent(GetMeshManager()->CreateMeshComponent(inPath));
}