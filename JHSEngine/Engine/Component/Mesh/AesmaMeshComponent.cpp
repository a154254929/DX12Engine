#include "AesmaMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CAesmaMeshComponent::CAesmaMeshComponent()
{
}

void CAesmaMeshComponent::CreateMesh(
    FMeshRenderingData& meshRenderingData,
    float inOuterRadius,
    float inInnerRadius,
    float inHeight,
    uint32_t inAxialSubdivision,
    uint32_t inHeightSubdivision
)
{
    inAxialSubdivision = max(inAxialSubdivision, 3);
    inHeightSubdivision = max(inHeightSubdivision, 1);
    inInnerRadius = min(inInnerRadius, inOuterRadius);

    float thetaValue = XM_2PI / inAxialSubdivision;
    float clip = 1.f / inHeightSubdivision;
    float heightClip = inHeight * clip;

    float topHeight = inHeight * .5f;
    float bottomHeight = -topHeight;
    float innerOuterRate = inInnerRadius * 1.0f / inOuterRadius;
    //top
    XMFLOAT3 topNormal(0.f, 1.f, 0.f);
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        float theta = i * thetaValue;
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(inOuterRadius * cosf(theta), topHeight, inOuterRadius * sinf(theta)),
            XMFLOAT4(0, i * 1.f / (inAxialSubdivision - 1), 1.f, 1.f),
            topNormal,
            XMFLOAT2(cosf(theta) * .5f + .5f, sinf(theta) * .5f + .5f)
        ));
    }
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        float theta = i * thetaValue;
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(inInnerRadius * cosf(theta), topHeight, inInnerRadius * sinf(theta)),
            XMFLOAT4(0, i * 1.f / (inAxialSubdivision - 1), 1.f, 1.f),
            topNormal,
            XMFLOAT2(cosf(theta) * .5f * innerOuterRate + .5f, sinf(theta) * .5f  * innerOuterRate + .5f)
        ));
    }
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        meshRenderingData.indexData.push_back(i);
        meshRenderingData.indexData.push_back(inAxialSubdivision + i);
        meshRenderingData.indexData.push_back((i + 1) % inAxialSubdivision + inAxialSubdivision);

        meshRenderingData.indexData.push_back(i);
        meshRenderingData.indexData.push_back((i + 1) % inAxialSubdivision + inAxialSubdivision);
        meshRenderingData.indexData.push_back((i + 1) % inAxialSubdivision);
    }

    //body Outer
    uint32_t topBodyIndexStart = inAxialSubdivision << 1;
    for (uint32_t i = 0; i <= inHeightSubdivision; ++i)
    {
        uint32_t roundStart = topBodyIndexStart + i * (inAxialSubdivision + 1);
        float height = (i * 1.0f / inHeightSubdivision - 0.5) * inHeight;
        for (uint32_t j = 0; j <= inAxialSubdivision; ++j)
        {
            float theta = j * thetaValue;
            meshRenderingData.vertexData.push_back(FVertex(
                XMFLOAT3(inOuterRadius * cosf(theta), height, inOuterRadius * sinf(theta)),
                XMFLOAT4(.25f, j * 1.f / inAxialSubdivision, 1.f, 1.f),
                XMFLOAT3(cosf(theta), 0, sinf(theta)),
                XMFLOAT2(j * 1.0f / inAxialSubdivision, i * 1.0f / inHeightSubdivision)
            ));
        }
        if (i > 0)
        {
            uint32_t lastRoundStart = roundStart - inAxialSubdivision - 1;
            for (uint32_t i = 0; i < inAxialSubdivision; ++i)
            {
                meshRenderingData.indexData.push_back(roundStart + i);
                meshRenderingData.indexData.push_back(lastRoundStart + i + 1);
                meshRenderingData.indexData.push_back(lastRoundStart + i);

                meshRenderingData.indexData.push_back(roundStart + i);
                meshRenderingData.indexData.push_back(roundStart + i + 1);
                meshRenderingData.indexData.push_back(lastRoundStart + i + 1);
            }
        }
    }

    //body Inner
    topBodyIndexStart += (inHeightSubdivision + 1) * (inAxialSubdivision + 1);
    for (uint32_t i = 0; i <= inHeightSubdivision; ++i)
    {
        uint32_t roundStart = topBodyIndexStart + i * (inAxialSubdivision + 1);
        float height = (i * 1.0f / inHeightSubdivision - 0.5) * inHeight;
        for (uint32_t j = 0; j <= inAxialSubdivision; ++j)
        {
            float theta = j * thetaValue;
            meshRenderingData.vertexData.push_back(FVertex(
                XMFLOAT3(inInnerRadius * cosf(theta), height, inInnerRadius * sinf(theta)),
                XMFLOAT4(.25f, j * 1.f / inAxialSubdivision, 1.f, 1.f),
                XMFLOAT3(-cosf(theta), 0, -sinf(theta)),
                XMFLOAT2(j * 1.0f / inAxialSubdivision, i * 1.0f / inHeightSubdivision)
            ));
        }
        if (i != 0)
        {
            uint32_t lastRoundStart = roundStart - inAxialSubdivision - 1;
            for (uint32_t i = 0; i < inAxialSubdivision; ++i)
            {
                meshRenderingData.indexData.push_back(roundStart + i);
                meshRenderingData.indexData.push_back(lastRoundStart + i);
                meshRenderingData.indexData.push_back(lastRoundStart + i + 1);

                meshRenderingData.indexData.push_back(roundStart + i);
                meshRenderingData.indexData.push_back(lastRoundStart + i + 1);
                meshRenderingData.indexData.push_back(roundStart + i + 1);
            }
        }
    }

    //bottom
    XMFLOAT3 bottomNormal(0.f, -1.f, 0.f);
    uint32_t bottomIndexStart = topBodyIndexStart + (inHeightSubdivision + 1) * (inAxialSubdivision + 1);
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        float theta = i * thetaValue;
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(inOuterRadius * cosf(theta), bottomHeight, inOuterRadius * sinf(theta)),
            XMFLOAT4(0, i * 1.f / (inAxialSubdivision - 1), 1.f, 1.f),
            bottomNormal,
            XMFLOAT2(cosf(theta) * .5f + .5f, sinf(theta) * .5f + .5f)
        ));
    }
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        float theta = i * thetaValue;
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(inInnerRadius * cosf(theta), bottomHeight, inInnerRadius * sinf(theta)),
            XMFLOAT4(0, i * 1.f / (inAxialSubdivision - 1), 1.f, 1.f),
            bottomNormal,
            XMFLOAT2(cosf(theta) * .5f * innerOuterRate + .5f, sinf(theta) * .5f  * innerOuterRate + .5f)
        ));
    }
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        meshRenderingData.indexData.push_back(bottomIndexStart + i);
        meshRenderingData.indexData.push_back(bottomIndexStart + (i + 1) % inAxialSubdivision + inAxialSubdivision);
        meshRenderingData.indexData.push_back(bottomIndexStart + inAxialSubdivision + i);

        meshRenderingData.indexData.push_back(bottomIndexStart + i);
        meshRenderingData.indexData.push_back(bottomIndexStart + (i + 1) % inAxialSubdivision);
        meshRenderingData.indexData.push_back(bottomIndexStart + (i + 1) % inAxialSubdivision + inAxialSubdivision);
    }
}