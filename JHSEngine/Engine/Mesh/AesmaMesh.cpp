#include "AesmaMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManager.h"
#include "../Core/Consttruction/MacroConstruction.h"
#include "../Component/Mesh/AesmaMeshComponent.h"

GAesmaMesh::GAesmaMesh()
{
    SetMeshComponent(ConstructionObject<CAesmaMeshComponent>());
}

void GAesmaMesh::Init()
{
    Super::Init();
}

void GAesmaMesh::Draw(float deltaTime)
{
    Super::Draw(deltaTime);
}

void GAesmaMesh::CreateMesh(
    float inOuterRadius,
    float inInnerRadius,
    float inHeight,
    uint32_t inAxialSubdivision,
    uint32_t inHeightSubdivision
)
{
    CREATE_RENDER_DATA(CAesmaMeshComponent, inOuterRadius, inInnerRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
    
    //SetMeshComponent(GetMeshManager()->CreateAesmaMeshComponent(inOuterRadius, inInnerRadius, inHeight, inAxialSubdivision, inHeightSubdivision));
}
