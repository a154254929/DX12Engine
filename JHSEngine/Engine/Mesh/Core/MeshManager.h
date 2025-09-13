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

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
public:
	CMesh* CreateBoxMesh(float inHeight, float inWidth, float inDepth);

	CMesh* CreateConeMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);

	CMesh* CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	CMesh* CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

	CMesh* CreateAesmaMesh(float inOuterRadius, float inInnerRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);

	CMesh* CreateDonutMesh(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision);

	CMesh* CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	CMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);
protected:

	FRenderingPipeline renderingPipeline;

	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12DescriptorHeap> cbvHeap;
	shared_ptr<FRenderingResourcesUpdate> objectConstants;
	shared_ptr<FRenderingResourcesUpdate> viewportConstants;

	vector<D3D12_INPUT_ELEMENT_DESC> inputElementDesc;
protected:
};
