#include "MeshManager.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "../AesmaMesh.h"
#include "../DonutMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Core/Viewport/ViewportTransformation.h"

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

CMesh* CMeshManager::CreateSphereMesh(float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CSphereMesh>(inRadius, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreateMesh(string& inPath)
{
    return CreateMesh<CCustomMesh>(inPath);
}

CMesh* CMeshManager::CreateBoxMesh(float inHeight, float inWidth, float inDepth)
{
    return CreateMesh<CBoxMesh>(inDepth, inDepth, inDepth);
}

CMesh* CMeshManager::CreateConeMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CConeMesh>(inRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreateCylinderMesh(float inTopRadius, float inBottomRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CCylinderMesh>(inTopRadius, inBottomRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreatePlaneMesh(float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
{
    return CreateMesh<CPlaneMesh>(inHeight, inWidth, inHeightSubdivide, inWidthSubdivide);
}

CMesh* CMeshManager::CreateAesmaMesh(float inOuterRadius, float inInnerRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CAesmaMesh>(inOuterRadius, inInnerRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreateDonutMesh(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
    return CreateMesh<CDonutMesh>(inDonutRadius, inThicknessRadius, inDonutAxialSubdivision, inThicknessAxialSubdivision);
}

template<class T, typename ...ParamTypes>
T* CMeshManager::CreateMesh(ParamTypes && ...params)
{
    T* myMesh = new T();

    FMeshRenderingData meshRenderingData;
    myMesh->CreateMesh(meshRenderingData, forward<ParamTypes>(params)...);

    myMesh->BeginInit();

    renderingPipeline.BuildMesh(myMesh, meshRenderingData);

    myMesh->Init();

    return myMesh;
}