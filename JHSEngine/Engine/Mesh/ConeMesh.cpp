#include "ConeMesh.h"

#include "../Component/Mesh/ConeMeshComponent.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"

GConeMesh::GConeMesh()
{
    SetMeshComponent(ConstructionObject<CConeMeshComponent>());
}

void GConeMesh::Init()
{
    Super::Init();
}

void GConeMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GConeMesh::CreateMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    CREATE_RENDER_DATA(CConeMeshComponent, inRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
    
    //SetMeshComponent(GetMeshManager()->CreateConeMeshComponent(inRadius, inHeight, inAxialSubdivision, inHeightSubdivision));
}
