#include "SphereMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"
#include  "../Component/Mesh/SphereMeshComponent.h"

GSphereMesh::GSphereMesh()
{
    SetMeshComponent(ConstructionObject<CSphereMeshComponent>());
}

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
    CREATE_RENDER_DATA(CSphereMeshComponent, inRadius, inAxialSubdivision, inHeightSubdivision, bReverse);
    
    //SetMeshComponent(GetMeshManager()->CreateSphereMeshComponent(inRadius, inAxialSubdivision, inHeightSubdivision, bReverse));
}
