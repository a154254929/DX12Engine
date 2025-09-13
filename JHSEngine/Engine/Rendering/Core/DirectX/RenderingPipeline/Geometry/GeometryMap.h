#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"


class FGeometry : public IDirectXDeviceInterface_Struct
{
public:
	bool bRenderingDataExistence(CMesh* inKey);
	void BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData);

	//构建模型
	void Build();

	/*后面会有变化*/
	UINT GetDrawObjectNumber() const { return describeMeshRenderingData.size(); }
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

	void BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData);

	void Build();

	void BuildDescriptorHeap();

	/*后面会有变化*/
	UINT GetDrawObjectNumber();
protected:
	map<int, FGeometry> geometrys;
	FDirectXDescriptorHeap descriptorHeap;
};