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
	GMesh* CreateBoxMesh(float inHeight, float inWidth, float inDepth);

	GMesh* CreateConeMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);

	GMesh* CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	GMesh* CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

	GMesh* CreateAesmaMesh(float inOuterRadius, float inInnerRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);

	GMesh* CreateDonutMesh(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision);

	GMesh* CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	GMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);
protected:

	FRenderingPipeline renderingPipeline;
	shared_ptr<FRenderingResourcesUpdate> viewportConstants;
};
