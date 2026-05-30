#include "CylinderMesh.h"

#include "../Component/Mesh/CylinderMeshComponent.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"

GCylinderMesh::GCylinderMesh()
{
    SetMeshComponent(ConstructionObject<CCylinderMeshComponent>());
}

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
    CREATE_RENDER_DATA(CCylinderMeshComponent, inTopRadius, inBottomRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
    
    //SetMeshComponent(GetMeshManager()->CreateCylinderMeshComponent(inTopRadius, inBottomRadius, inHeight, inAxialSubdivision, inHeightSubdivision));
}
