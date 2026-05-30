#include "DonutMesh.h"

#include "../Component/Mesh/DonutMeshComponent.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"

GDonutMesh::GDonutMesh()
{
    SetMeshComponent(ConstructionObject<CDonutMeshComponent>());
}

void GDonutMesh::Init()
{
    Super::Init();
}

void GDonutMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GDonutMesh::CreateMesh(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
    CREATE_RENDER_DATA(CDonutMeshComponent, inDonutRadius, inThicknessRadius, inDonutAxialSubdivision, inThicknessAxialSubdivision);
    
    //SetMeshComponent(GetMeshManager()->CreateDonutMeshComponent(inDonutRadius, inThicknessRadius, inDonutAxialSubdivision, inThicknessAxialSubdivision));
}
