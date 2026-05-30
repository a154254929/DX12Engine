#include "CustomMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"
#include "../Component/Mesh/CustomMeshComponent.h"

GCustomMesh::GCustomMesh()
{
    SetMeshComponent(ConstructionObject<CCustomMeshComponent>());
}

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
    CREATE_RENDER_DATA(CCustomMeshComponent, inPath);
    
    //SetMeshComponent(GetMeshManager()->CreateMeshComponent(inPath));
}
