#include "PlaneMesh.h"

#include "../Component/Mesh/PlaneMeshComponent.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"

GPlaneMesh::GPlaneMesh()
{
    SetMeshComponent(ConstructionObject<CPlaneMeshComponent>());
}

void GPlaneMesh::Init()
{
    Super::Init();
}

void GPlaneMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GPlaneMesh::CreateMesh(float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
{
    CREATE_RENDER_DATA(CPlaneMeshComponent, inHeight, inWidth, inHeightSubdivide, inWidthSubdivide);
    
    //SetMeshComponent(GetMeshManager()->CreatePlaneMeshComponent(inHeight, inWidth, inHeightSubdivide, inWidthSubdivide));
}
