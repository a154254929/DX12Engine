#include "DonutMesh.h"
#include "Core/MeshType.h"

void CDonutMesh::Init()
{
	Super::Init();
}

void CDonutMesh::Draw(float deltaTime)
{
	Super::Draw(deltaTime);
}

void CDonutMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
	Super::BuildMesh(inRenderingData);
}

CDonutMesh* CDonutMesh::CreateMesh(FMeshRenderingData& meshRenderingData, float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
	inDonutAxialSubdivision = max(inDonutAxialSubdivision, 3);
	inThicknessAxialSubdivision = max(inThicknessAxialSubdivision, 3);

	inThicknessRadius = min(inThicknessRadius, inDonutRadius * 0.5);

	float donutThetaValue = XM_2PI / inDonutAxialSubdivision;
	float thicknessThetaValue = XM_2PI / inThicknessAxialSubdivision;

	for (int i = 0; i < inDonutAxialSubdivision; ++i)
	{
		float centerTheta = i * donutThetaValue;
		XMFLOAT3 circleCenter = XMFLOAT3(cos(centerTheta) * donutThetaValue, 0, sin(centerTheta) * donutThetaValue);
		XMFLOAT3 circleVector = XMFLOAT3(-cos(centerTheta), 0, -sin(centerTheta));
		for (int j = 0; j < inThicknessAxialSubdivision; ++j)
		{
			float theta = j * thicknessThetaValue;
			XMFLOAT3 normalF3 = XMFLOAT3(circleVector.x * cos(theta), sin(theta), circleVector.z * cos(theta));

			meshRenderingData.vertexData.push_back(FVertex(
				XMFLOAT3(normalF3.x * inThicknessRadius + circleVector.x, normalF3.y * inThicknessRadius, normalF3.z * inThicknessRadius + circleVector.z),
				XMFLOAT4(i * 1.f / (inDonutAxialSubdivision - 1), j * 1.f / (inThicknessAxialSubdivision - 1), 1.f, 1.f)
			));
			meshRenderingData.vertexData[i * inThicknessAxialSubdivision + j].normal = normalF3;
		}

		for (int j = 0; j < inThicknessAxialSubdivision; ++j)
		{
			meshRenderingData.indexData.push_back(i * inThicknessAxialSubdivision + j);
			meshRenderingData.indexData.push_back(i * inThicknessAxialSubdivision + ((j + 1) % inThicknessAxialSubdivision));
			meshRenderingData.indexData.push_back(((i + 1) % inThicknessAxialSubdivision) * inThicknessAxialSubdivision + j);

			meshRenderingData.indexData.push_back(i * inThicknessAxialSubdivision + j);
			meshRenderingData.indexData.push_back(((i + 1) % inThicknessAxialSubdivision) * inThicknessAxialSubdivision + j);
			meshRenderingData.indexData.push_back(((i + 1) % inThicknessAxialSubdivision) * inThicknessAxialSubdivision + ((j + 1) % inThicknessAxialSubdivision));
		}
	}

	CDonutMesh* donutMesh = new CDonutMesh;
	donutMesh->BuildMesh(&meshRenderingData);

	donutMesh->Init();

	return donutMesh;
}