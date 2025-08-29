#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Core/ViewportInfo.h"

class FRenderingResourcesUpdate;

class CMeshManager
	:public CCoreMinimalObject
	, public IRenderingInterface
	, public IDirectXDeviceInterface
{
public:
	CMeshManager();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);
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

	CMesh* CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	CMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);
protected:
	ComPtr<ID3DBlob> cpuVertexBufferPtr;
	ComPtr<ID3DBlob> cpuIndexBufferPtr;

	ComPtr<ID3D12Resource> gpuVertexBufferPtr;
	ComPtr<ID3D12Resource> gpuIndexBufferPtr;

	ComPtr<ID3D12Resource> vertexBufferTmpPtr;
	ComPtr<ID3D12Resource> indexBufferTmpPtr;

	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12DescriptorHeap> cbvHeap;
	shared_ptr<FRenderingResourcesUpdate> objectConstants;

	ComPtr<ID3D12PipelineState> pipelineStatePSO;

	FShader vertexShader;
	FShader pixelShader;

	vector<D3D12_INPUT_ELEMENT_DESC> inputElementDesc;
protected:
	UINT vertexSizeInBytes;
	UINT vertexStrideInBytes;

	UINT indexSizeInBytes;
	DXGI_FORMAT indexFormat;
	UINT indexSize;

	XMFLOAT4X4 worldMatrix;
};
