#include "PlaneMesh.h"
#include "Core/MeshType.h"

void GPlaneMesh::Init()
{
	Super::Init();
}

void GPlaneMesh::Draw(float deltaTime)
{
	Super::Draw(deltaTime);
}

void GPlaneMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
	Super::BuildMesh(inRenderingData);
}

GPlaneMesh* GPlaneMesh::CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
{
	auto subdivideValue = [&](float inValue, float inSubdivideValue)->float
	{
			if (inValue <= 0)
			{
				return inValue;
			}
			return inValue * 1.f / inWidthSubdivide;
	};

	float halfHeight = inHeight * .5f;
	float halfWidth = inWidth * .5f;
	float heightSubdivide = subdivideValue(inHeight, inHeightSubdivide);
	float widthSubdivide = subdivideValue(inWidth, inWidthSubdivide);

	//构建顶点
	for (uint32_t i = 0; i <= inHeightSubdivide; ++i)
	{
		for (uint32_t j = 0; j <= inWidthSubdivide; ++j)
		{
			meshRenderingData.vertexData.push_back(FVertex(
				XMFLOAT3(-halfWidth + heightSubdivide * i, 0, -halfWidth + widthSubdivide * j),
				XMFLOAT4(i * 1.0f / inHeightSubdivide, j * 1.0f / inWidthSubdivide, 1.f, 1.f),
				XMFLOAT3(.0f, 1.0f, .0f)
			));
			if (i != 0 && j != inWidthSubdivide)
			{
				meshRenderingData.indexData.push_back(i * (inWidthSubdivide + 1) + j);
				meshRenderingData.indexData.push_back((i - 1) * (inWidthSubdivide + 1) + j);
				meshRenderingData.indexData.push_back((i - 1) * (inWidthSubdivide + 1) + j + 1);

				meshRenderingData.indexData.push_back(i * (inWidthSubdivide + 1) + j);
				meshRenderingData.indexData.push_back((i - 1) * (inWidthSubdivide + 1) + j + 1);
				meshRenderingData.indexData.push_back(i * (inWidthSubdivide + 1) + j + 1);
			}
		}
	}

	GPlaneMesh* planeMesh = new GPlaneMesh;
	planeMesh->BuildMesh(&meshRenderingData);

	planeMesh->Init();

	return planeMesh;
}