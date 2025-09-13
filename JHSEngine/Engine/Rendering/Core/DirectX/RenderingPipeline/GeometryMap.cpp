#include "GeometryMap.h"

FGeometryMap::FGeometryMap()
{
	geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::BuildMesh(CMesh* inMesh, const FMeshRenderingData* inRenderingData)
{
	FGeometry& geometry = geometrys[0];
	geometry.BuildMesh(inMesh, inRenderingData);
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