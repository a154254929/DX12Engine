#include "MeshManager.h"
#include "../../Config/EngineRenderConfig.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Core/Viewport/ViewportTransformation.h"
#include "../../Component/Mesh/BoxMeshComponent.h"
#include "../../Component/Mesh/ConeMeshComponent.h"
#include "../../Component/Mesh/CustomMeshComponent.h"
#include "../../Component/Mesh/CylinderMeshComponent.h"
#include "../../Component/Mesh/PlaneMeshComponent.h"
#include "../../Component/Mesh/SphereMeshComponent.h"
#include "../../Component/Mesh/AesmaMeshComponent.h"
#include "../../Component/Mesh/DonutMeshComponent.h"
#include "../../Core/Consttruction/MeshConstruction.h"

CMeshManager::CMeshManager()
{

}

void CMeshManager::Init()
{
}

void CMeshManager::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
    renderingPipeline.UpdateCalculations(deltaTime, viewportInfo);
}

void CMeshManager::BuildMesh()
{
    renderingPipeline.BuildPipeline();
}

void CMeshManager::PreDraw(float deltaTime)
{
    renderingPipeline.PreDraw(deltaTime);
}

void CMeshManager::Draw(float deltaTime)
{
    renderingPipeline.Draw(deltaTime);
}

void CMeshManager::PostDraw(float deltaTime)
{
    renderingPipeline.PostDraw(deltaTime);
}

CMeshComponent* CMeshManager::CreateSphereMeshComponent(float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision, bool bReverse)
{
    return MeshConstruction::CreateMeshComponent<CSphereMeshComponent>(this, inRadius, inAxialSubdivision, inHeightSubdivision, bReverse);
}

CMeshComponent* CMeshManager::CreateMeshComponent(string& inPath)
{
    return MeshConstruction::CreateMeshComponent<CCustomMeshComponent>(this, inPath);
}

CMeshComponent* CMeshManager::CreateBoxMeshComponent(float inHeight, float inWidth, float inDepth)
{
    return MeshConstruction::CreateMeshComponent<CBoxMeshComponent>(this, inDepth, inDepth, inDepth);
}

CMeshComponent* CMeshManager::CreateConeMeshComponent(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CConeMeshComponent>(this, inRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMeshComponent* CMeshManager::CreateCylinderMeshComponent(float inTopRadius, float inBottomRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CCylinderMeshComponent>(this, inTopRadius, inBottomRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMeshComponent* CMeshManager::CreatePlaneMeshComponent(float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
{
    return MeshConstruction::CreateMeshComponent<CPlaneMeshComponent>(this, inHeight, inWidth, inHeightSubdivide, inWidthSubdivide);
}

CMeshComponent* CMeshManager::CreateAesmaMeshComponent(float inOuterRadius, float inInnerRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CAesmaMeshComponent>(this, inOuterRadius, inInnerRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMeshComponent* CMeshManager::CreateDonutMeshComponent(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
    return MeshConstruction::CreateMeshComponent<CDonutMeshComponent>(this, inDonutRadius, inThicknessRadius, inDonutAxialSubdivision, inThicknessAxialSubdivision);
}

template<class T, typename ...ParamTypes>
T* CMeshManager::CreateMeshComponent(ParamTypes && ...params)
{
    T* myMeshComp = new T();
    
    size_t meshHashKey = 0;
    myMeshComp->BuildKey(meshHashKey, forward<ParamTypes>(params)...);
    FRenderingData renderingData;
    if (renderingPipeline.FindMeshRenderingData(meshHashKey, renderingData, (int)myMeshComp->GetRenderLayerType()))
    {
        renderingPipeline.DuplicateMesh(myMeshComp, renderingData);
    }
    else
    {

        FMeshRenderingData meshRenderingData;
        myMeshComp->CreateMesh(meshRenderingData, forward<ParamTypes>(params)...);

        renderingPipeline.BuildMesh(meshHashKey, myMeshComp, meshRenderingData);

        myMeshComp->Init();
    }
    return myMeshComp;
}