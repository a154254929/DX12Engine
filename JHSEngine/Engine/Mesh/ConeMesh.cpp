#include "ConeMesh.h"
#include "Core/MeshType.h"

void CConeMesh::Init()
{
	Super::Init();
}

void CConeMesh::Draw(float deltaTime)
{
	Super::Draw(deltaTime);
}

void CConeMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
	Super::BuildMesh(inRenderingData);
}

CConeMesh* CConeMesh::CreateMesh(FMeshRenderingData& meshRenderingData, float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
	inAxialSubdivision = max(inAxialSubdivision, 3);
	inHeightSubdivision = max(inHeightSubdivision, 1);

	float thetaValue = XM_2PI / inAxialSubdivision;
	float topHeight = inHeight * .5f;
	float bottomHeight = -topHeight;
	float clip = 1.f / (inHeightSubdivision - 1);
	float heightClip = inHeight * clip;
	float radiusClip = inRadius * clip;

	//top
	meshRenderingData.vertexData.push_back(FVertex(
		XMFLOAT3(0.f, topHeight, 0.f),
		XMFLOAT4(0.f, 0.f, 1.f, 1.f)
	));
	XMFLOAT3 topNormal(0.f, 1.f, 0.f);
	meshRenderingData.vertexData[0].normal = topNormal;
	for (uint32_t i = 0; i < inAxialSubdivision; ++i)
	{
	}
	for (uint32_t j = 0; j < inAxialSubdivision; ++j)
	{
		meshRenderingData.indexData.push_back(j + 1);
		meshRenderingData.indexData.push_back(0);
		meshRenderingData.indexData.push_back((j + 1) % inAxialSubdivision + 1);
	}

	//body
	for (uint32_t i = 1; i < inHeightSubdivision; ++i)
	{
		uint32_t roundStart = (i - 1) * inAxialSubdivision + 1;
		float height = topHeight - heightClip * i;
		float redius = radiusClip * i;
		for (uint32_t j = 0; j < inAxialSubdivision; ++j)
		{
			float theta = j * thetaValue;
			meshRenderingData.vertexData.push_back(FVertex(
				XMFLOAT3(redius * cosf(theta), height, redius * sinf(theta)),
				XMFLOAT4(.25f, j * 1.f / (inAxialSubdivision - 1), 1.f, 1.f)
			));
			meshRenderingData.vertexData[roundStart + j].normal = XMFLOAT3(cosf(theta), 0, sinf(theta));
		}
		if (i == 1)
		{
			for (uint32_t j = 0; j < inAxialSubdivision; ++j)
			{
				meshRenderingData.indexData.push_back(j + roundStart);
				meshRenderingData.indexData.push_back(0);
				meshRenderingData.indexData.push_back((j + 1) % inAxialSubdivision + roundStart);
			}
		}
		else
		{
			uint32_t lastRoundStart = roundStart - inAxialSubdivision;
			for (uint32_t j = 0; j < inAxialSubdivision; ++j)
			{
				meshRenderingData.indexData.push_back(roundStart + j);
				meshRenderingData.indexData.push_back(lastRoundStart + j);
				meshRenderingData.indexData.push_back(lastRoundStart + ((j + 1) % inAxialSubdivision));

				meshRenderingData.indexData.push_back(roundStart + j);
				meshRenderingData.indexData.push_back(lastRoundStart + ((j + 1) % inAxialSubdivision));
				meshRenderingData.indexData.push_back(roundStart + ((j + 1) % inAxialSubdivision));
			}
		}
	}

	//bottom
	XMFLOAT3 bottomNormal(0.f, -1.f, 0.f);
	uint32_t bottomIndexStart = inAxialSubdivision * (inHeightSubdivision - 1) + 1;
	for (uint32_t i = 0; i < inAxialSubdivision; ++i)
	{
		float theta = i * thetaValue;
		meshRenderingData.vertexData.push_back(FVertex(
			XMFLOAT3(inRadius * cosf(theta), bottomHeight, inRadius * sinf(theta)),
			XMFLOAT4(1.f, i * 1.f / (inAxialSubdivision - 1), 1.f, 1.f)
		));
		meshRenderingData.vertexData[bottomIndexStart + i].normal = bottomNormal;
	}
	meshRenderingData.vertexData.push_back(FVertex(
		XMFLOAT3(0.f, bottomHeight, 0.f),
		XMFLOAT4(1.f, 1.f, 1.f, 1.f)
	));
	uint32_t endIndex = inAxialSubdivision + bottomIndexStart;
	meshRenderingData.vertexData[endIndex].normal = bottomNormal;
	for (uint32_t j = 0; j < inAxialSubdivision; ++j)
	{
		meshRenderingData.indexData.push_back(endIndex);
		meshRenderingData.indexData.push_back(bottomIndexStart + j);
		meshRenderingData.indexData.push_back(bottomIndexStart + (j + 1) % inAxialSubdivision);
	}
	CConeMesh* coneMesh = new CConeMesh;
	coneMesh->BuildMesh(&meshRenderingData);

	coneMesh->Init();

	return coneMesh;
}