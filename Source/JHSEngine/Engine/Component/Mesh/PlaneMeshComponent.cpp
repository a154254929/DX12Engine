#include "PlaneMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPlaneMeshComponent::CPlaneMeshComponent()
{
}

void CPlaneMeshComponent::CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
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
                XMFLOAT3(.0f, 1.0f, .0f),
                XMFLOAT2(j * 1.0f / inWidthSubdivide, i * 1.0f / inHeightSubdivide)
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
}

void CPlaneMeshComponent::BuildKey(size_t& meshHashKey, float inHeight, float inWidth, uint32_t inHeightSubdivide,
    uint32_t inWidthSubdivide)
{
    std::hash<float> floatHash;
    std::hash<int> intHash;
    
    meshHashKey = 6;
    meshHashKey += floatHash(inHeight);
    meshHashKey += floatHash(inWidth);
    meshHashKey += intHash._Do_hash(inHeightSubdivide);
    meshHashKey += intHash._Do_hash(inWidthSubdivide);
}
