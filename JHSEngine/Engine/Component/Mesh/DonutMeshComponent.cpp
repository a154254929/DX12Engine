#include "DonutMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CDonutMeshComponent::CDonutMeshComponent()
{
}

void CDonutMeshComponent::CreateMesh(FMeshRenderingData& meshRenderingData, float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
    inDonutAxialSubdivision = max(inDonutAxialSubdivision, 3);
    inThicknessAxialSubdivision = max(inThicknessAxialSubdivision, 3);

    inThicknessRadius = min(inThicknessRadius, inDonutRadius * 0.5);

    float donutThetaValue = XM_2PI / inDonutAxialSubdivision;
    float thicknessThetaValue = XM_2PI / inThicknessAxialSubdivision;

    for (int i = 0; i <= inDonutAxialSubdivision; ++i)
    {
        float centerTheta = i * donutThetaValue;
        float u = i * 1.f / inDonutAxialSubdivision;
        XMFLOAT3 circleCenter = XMFLOAT3(cos(centerTheta) * inDonutRadius, 0, sin(centerTheta) * inDonutRadius);
        XMFLOAT3 circleVector = XMFLOAT3(-cos(centerTheta), 0, -sin(centerTheta));
        for (int j = 0; j <= inThicknessAxialSubdivision; ++j)
        {
            float theta = j * thicknessThetaValue;
            float v = j * 1.f / inThicknessAxialSubdivision;
            XMFLOAT3 normalF3 = XMFLOAT3(circleVector.x * cos(theta), sin(theta), circleVector.z * cos(theta));

            meshRenderingData.vertexData.push_back(FVertex(
                XMFLOAT3(normalF3.x * inThicknessRadius + circleCenter.x, normalF3.y * inThicknessRadius + circleCenter.y, normalF3.z * inThicknessRadius + circleCenter.z),
                XMFLOAT4(u, v , 1.f, 1.f),
                normalF3,
                XMFLOAT2(u, v)
            ));
        }

        if (i >= 1)
        {
            for (int j = 0; j < inThicknessAxialSubdivision; ++j)
            {
                meshRenderingData.indexData.push_back(i * (inThicknessAxialSubdivision + 1) + j);
                meshRenderingData.indexData.push_back(i * (inThicknessAxialSubdivision + 1) + j + 1);
                meshRenderingData.indexData.push_back((i - 1) * (inThicknessAxialSubdivision + 1) + j);

                meshRenderingData.indexData.push_back(i * (inThicknessAxialSubdivision + 1) + j + 1);
                meshRenderingData.indexData.push_back((i - 1) * (inThicknessAxialSubdivision + 1) + j + 1);
                meshRenderingData.indexData.push_back((i - 1) * (inThicknessAxialSubdivision + 1) + j);
            }
        }
    }
}