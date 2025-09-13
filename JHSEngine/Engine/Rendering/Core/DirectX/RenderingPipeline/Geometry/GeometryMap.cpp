#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"

FGeometryMap::FGeometryMap()
{
	geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData)
{
	FGeometry& geometry = geometrys[0];
	geometry.BuildMesh(inMesh, inMeshData);
}

void FGeometryMap::Build()
{
	//构建模型
	for (auto& tmp : geometrys)
	{
		tmp.second.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1是因为有摄像机
	descriptorHeap.Build(GetDrawObjectNumber() + 1);
}

UINT FGeometryMap::GetDrawObjectNumber()
{
	return geometrys[0].GetDrawObjectNumber();
}

bool FGeometry::bRenderingDataExistence(CMesh* inKey)
{
	for (auto& tmp : describeMeshRenderingData)
	{
		if (tmp.mesh == inKey)
			return true;
	}
	return false;
}

void FGeometry::BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData)
{
	if (!bRenderingDataExistence(inMesh))
	{
		describeMeshRenderingData.push_back(FRenderingData());
		FRenderingData& inRenderingData = describeMeshRenderingData[describeMeshRenderingData.size() - 1];

		//基础信息记录
		inRenderingData.mesh = inMesh;
		inRenderingData.indexSize += inMeshData.indexData.size();
		inRenderingData.vertexSize += inMeshData.vertexData.size();

		inRenderingData.indexOffsetPosition = meshRenderingData.indexData.size();
		inRenderingData.vertexOffsetPosition = meshRenderingData.vertexData.size();

		//索引的合并
		meshRenderingData.indexData.insert(
			meshRenderingData.indexData.end()
			, inMeshData.indexData.begin()
			, inMeshData.indexData.end()
		);
		//顶点的合并
		meshRenderingData.vertexData.insert(
			meshRenderingData.vertexData.end()
			, inMeshData.vertexData.begin()
			, inMeshData.vertexData.end()
		);
	}
}

void FGeometry::Build()
{
	//构建模型
	UINT vertexSizeInBytes = meshRenderingData.GetVertexSizeInBytes();
	UINT indexSizeInBytes = meshRenderingData.GetIndexSizeInBytes();

	ANALYSIS_HRESULT(D3DCreateBlob(
		vertexSizeInBytes,
		&cpuVertexBufferPtr
	));
	memcpy(
		cpuVertexBufferPtr->GetBufferPointer(),
		meshRenderingData.vertexData.data(),
		vertexSizeInBytes
	);

	ANALYSIS_HRESULT(D3DCreateBlob(
		indexSizeInBytes,
		&cpuIndexBufferPtr
	));
	memcpy(
		cpuIndexBufferPtr->GetBufferPointer(),
		meshRenderingData.indexData.data(),
		indexSizeInBytes
	);

	ConstructBuffer::FConstructBuffer constructBuffer;
	gpuVertexBufferPtr = constructBuffer.ConstructDefaultBuffer(
		vertexBufferTmpPtr,
		meshRenderingData.vertexData.data(),
		vertexSizeInBytes
	);
	gpuIndexBufferPtr = constructBuffer.ConstructDefaultBuffer(
		indexBufferTmpPtr,
		meshRenderingData.indexData.data(),
		indexSizeInBytes
	);
}
