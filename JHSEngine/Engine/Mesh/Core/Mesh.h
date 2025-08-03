#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 world;

	static XMFLOAT4X4 IdentityMatrix4x4();
};

class FMesh : public IRenderingInterface
{
public:
	FMesh();
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	static FMesh* CreateMesh(const FMeshRenderingData* inRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
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
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectMatrix;
};