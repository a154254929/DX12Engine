#include "BoxMesh.h"

#include "../Component/Mesh/BoxMeshComponent.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"

GBoxMesh::GBoxMesh()
{
    FCreateObjectParam param;
    param.owner = this;
    SetMeshComponent(ConstructionObject<CBoxMeshComponent>(param));
}

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
    CREATE_RENDER_DATA(CBoxMeshComponent,  inHeight, inWidth, inDepth);
    
    //SetMeshComponent(GetMeshManager()->CreateBoxMeshComponent(inHeight, inWidth, inDepth));
}
