#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"

class FRenderingResourcesUpdate;

class CMeshManager
    :public CCoreMinimalObject
    , public IRenderingInterface
    , public IDirectXDeviceInterface
{
public:
    CMeshManager();

    virtual void Init();

    virtual void BuildMesh();
    virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);

    virtual void PreDraw(float deltaTime);
    virtual void Draw(float deltaTime);
    virtual void PostDraw(float deltaTime);
public:
    CMeshComponent* CreateBoxMeshComponent(const FCreateObjectParam& inOwnerParam, float inHeight, float inWidth, float inDepth);

    CMeshComponent* CreateConeMeshComponent(const FCreateObjectParam& inOwnerParam, float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);

    CMeshComponent* CreateCylinderMeshComponent(const FCreateObjectParam& inOwnerParam, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

    CMeshComponent* CreatePlaneMeshComponent(const FCreateObjectParam& inOwnerParam, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

    CMeshComponent* CreateAesmaMeshComponent(const FCreateObjectParam& inOwnerParam, float inOuterRadius, float inInnerRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);

    CMeshComponent* CreateDonutMeshComponent(const FCreateObjectParam& inOwnerParam, float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision);

    CMeshComponent* CreateSphereMeshComponent(const FCreateObjectParam& inOwnerParam, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse = false);

    CMeshComponent* CreateMeshComponent(const FCreateObjectParam& inOwnerParam, string& InPath);
    
    FRenderingPipeline& GetRenderingPipeline() {return renderingPipeline;};

protected:
    template<class T, typename ...ParamTypes>
    T* CreateMeshComponent(ParamTypes &&...Params);
protected:

    FRenderingPipeline renderingPipeline;
    shared_ptr<FRenderingResourcesUpdate> viewportConstants;
};
