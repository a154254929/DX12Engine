#include "SphereMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CSphereMeshComponent::CSphereMeshComponent()
{
    
}

void CSphereMeshComponent::CreateMesh(FMeshRenderingData& meshRenderingData, float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    inAxialSubdivision = max(inAxialSubdivision, 3);
    inHeightSubdivision = max(inHeightSubdivision, 2);

    float thetaValue = XM_2PI / inAxialSubdivision;
    float betaValue = XM_PI / inHeightSubdivision;

    meshRenderingData.vertexData.push_back(FVertex(
        XMFLOAT3(0.f, inRadius, 0.f),
        XMFLOAT4( 0.f, 0.f, 1.f, 1.f),
        XMFLOAT3(0.f, 1, 0.f),
        XMFLOAT2(0.5f, 0.f)
    ));
    for (uint32_t i = 1; i < inHeightSubdivision; ++i)
    {
        float beta = betaValue * i;
        float y = cosf(beta) * inRadius;
        float layerRadius = sinf(beta) * inRadius;

        uint32_t roundStart = (i - 1) * inAxialSubdivision + 1;
        for (uint32_t j = 0; j < inAxialSubdivision; ++j)
        {
            float theta = j * thetaValue;
            meshRenderingData.vertexData.push_back(FVertex(
                XMFLOAT3(layerRadius * cosf(theta), y, layerRadius * sinf(theta)),
                XMFLOAT4(i * 1.f / (inHeightSubdivision - 1), j * 1.f / (inAxialSubdivision - 1), 1.f, 1.f)
            ));
            FVertex& curVert = meshRenderingData.vertexData[roundStart + j];
            XMVECTOR vertexPos = XMLoadFloat3(&curVert.position);
            XMStoreFloat3(&curVert.normal, XMVector3Normalize(vertexPos));

            //U方向切线
            curVert.utangent.x = -layerRadius * sinf(theta);
            curVert.utangent.y = 0;
            curVert.utangent.z = layerRadius * cosf(theta);
            curVert.texcoord = XMFLOAT2(theta / XM_2PI, beta / XM_PI) ;
            XMVECTOR utangent = XMLoadFloat3(&curVert.utangent);
            XMStoreFloat3(&curVert.utangent, XMVector3Normalize(utangent));
        }
        if (i == 1)
        {
            for (uint32_t j = 0; j < inAxialSubdivision; ++j)
            {
                meshRenderingData.indexData.push_back(roundStart + j);
                meshRenderingData.indexData.push_back(0);
                meshRenderingData.indexData.push_back(roundStart + ((1 + j) % inAxialSubdivision));
            }
        }
        else
        {
            uint32_t lastRoundStart = (i - 2) * inAxialSubdivision + 1;
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
    meshRenderingData.vertexData.push_back(FVertex(
        XMFLOAT3(0.f, -inRadius, 0.f),
        XMFLOAT4(Colors::White),
        XMFLOAT3(0.f, -1.f, 0.f),
        XMFLOAT2(0.5f, 1.f)
    ));
    uint32_t finalIndex = (inHeightSubdivision - 1) * inAxialSubdivision + 1;
    uint32_t finalRoundStart = (inHeightSubdivision - 2) * inAxialSubdivision + 1;
    for (uint32_t i = 0; i < inAxialSubdivision; ++i)
    {
        meshRenderingData.indexData.push_back(finalIndex);
        meshRenderingData.indexData.push_back(finalRoundStart + i);
        meshRenderingData.indexData.push_back(finalRoundStart + ((i + 1) % inAxialSubdivision));
    }
}