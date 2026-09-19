#include "CustomMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"
#include "../Component/Mesh/CustomMeshComponent.h"

GCustomMesh::GCustomMesh()
{
    FCreateObjectParam param;
    param.owner = this;
    SetMeshComponent(ConstructionObject<CCustomMeshComponent>(param));
}

void GCustomMesh::Init()
{
    Super::Init();
}

void GCustomMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GCustomMesh::CreateMesh(const string& inPath)
{
    CREATE_RENDER_DATA(CCustomMeshComponent, inPath);
    
    //SetMeshComponent(GetMeshManager()->CreateMeshComponent(inPath));
}
