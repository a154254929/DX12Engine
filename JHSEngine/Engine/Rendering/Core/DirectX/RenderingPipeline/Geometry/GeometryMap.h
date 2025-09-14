#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferView.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"


class FGeometry : public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;
public:
	bool bRenderingDataExistence(CMesh* inKey);
	void BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData);

	//构建模型
	void Build();

	/*后面会有变化*/
	UINT GetDrawObjectNumber() const { return describeMeshRenderingData.size(); }

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
	ComPtr<ID3DBlob> cpuVertexBufferPtr;
	ComPtr<ID3DBlob> cpuIndexBufferPtr;

	ComPtr<ID3D12Resource> gpuVertexBufferPtr;
	ComPtr<ID3D12Resource> gpuIndexBufferPtr;

	ComPtr<ID3D12Resource> vertexBufferTmpPtr;
	ComPtr<ID3D12Resource> indexBufferTmpPtr;

	FMeshRenderingData meshRenderingData;
	vector<FRenderingData> describeMeshRenderingData;
};

class FGeometryMap : public IDirectXDeviceInterface
{
public:
	FGeometryMap();

	void PreDraw(float deltaTime);
	void Draw(float deltaTime);
	void PostDraw(float deltaTime);

	virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);

	void BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData);

	void Build();
	//描述堆
	void BuildDescriptorHeap();

	//常量缓冲区
	void BuildObjectConstantBuffer();

	//构建视口常量缓冲区
	void BuildViewportConstantBuffer();

	/*后面会有变化*/
	UINT GetDrawObjectNumber();

public:
	void DrawViewport(float deltaTime);

	void DrawMesh(float deltaTime);
public:
	ID3D12DescriptorHeap* GetHeap() const { return descriptorHeap.GetHeap(); }
protected:
	map<int, FGeometry> geometrys;
	FDirectXDescriptorHeap descriptorHeap;

	FConstantBufferView objectConstantBufferView;
	FConstantBufferView viewportConstantBufferView;
};