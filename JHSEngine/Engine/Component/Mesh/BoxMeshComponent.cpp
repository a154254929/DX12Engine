#include "BoxMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CBoxMeshComponent::CBoxMeshComponent()
{
}

void CBoxMeshComponent::CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, float inDepth)
{
    float halfHeight = inHeight * .5f;
    float halfWidth = inWidth * .5f;
    float halfDepth = inDepth * .5f;

    //构建顶点
    int vertexCount = 0;
    for (int i = 0; i < 6; i++)
    {
        bool isUpDown = (i >> 1) == 0;
        bool isLeftRight = (i >> 1) == 1;
        bool isFrontBack = (i >> 1) == 2;
        float sign = (i & 1) ? -1.0f : 1.0f;
        XMFLOAT3 normal = XMFLOAT3(
            isLeftRight ? sign : 0, 
            isUpDown ? sign : 0,
            isFrontBack ? sign : 0
            );
        // 构建面的局部坐标系：需要两个与法线正交的切向量
        float halfU, halfV;
        XMFLOAT3 tangent, bitangent;
    
        if (isUpDown)
        {
            // 法线沿Y轴，面在XZ平面
            halfU = inWidth * .5f;  // tangent方向半长
            halfV = inDepth * .5f;  // bitangent方向半长
            if (sign > 0)
            {
                // (0,1,0): Z × X = Y ✅ (左手坐标系)
                tangent = XMFLOAT3(0, 0, 1);   // 原来是 (1,0,0)
                bitangent = XMFLOAT3(1, 0, 0); // 原来是 (0,0,1)
            }
            else
            {
                // (0,-1,0): X × Z = -Y ✅ (左手坐标系)
                tangent = XMFLOAT3(1, 0, 0);   // 原来是 (0,0,1)
                bitangent = XMFLOAT3(0, 0, 1); // 原来是 (1,0,0)
            }
        }
        else if (isLeftRight)
        {
            // 法线沿X轴，面在YZ平面
            halfU = inHeight * .5f;
            halfV = inDepth * .5f;
            if (sign > 0)
            {
                // (1,0,0): Y × Z = X
                tangent = XMFLOAT3(0, 1, 0);
                bitangent = XMFLOAT3(0, 0, 1);
            }
            else
            {
                // (-1,0,0): Z × Y = -X
                tangent = XMFLOAT3(0, 0, 1);
                bitangent = XMFLOAT3(0, 1, 0);
            }
        }
        else // isFrontBack
        {
            // 法线沿Z轴，面在XY平面
            halfU = inWidth * .5f;
            halfV = inHeight * .5f;
            if (sign > 0)
            {
                // (0,0,1): X × Y = Z
                tangent = XMFLOAT3(1, 0, 0);
                bitangent = XMFLOAT3(0, 1, 0);
            }
            else
            {
                // (0,0,-1): Y × X = -Z
                tangent = XMFLOAT3(0, 1, 0);
                bitangent = XMFLOAT3(1, 0, 0);
            }
        }
    
        // 面的中心点
        XMFLOAT3 center = XMFLOAT3(
            normal.x * inWidth * .5f,
            normal.y * inHeight * .5f,
            normal.z * inDepth * .5f
        );
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(
                center.x - halfU * tangent.x - halfV * bitangent.x,
                center.y - halfU * tangent.y - halfV * bitangent.y,
                center.z - halfU * tangent.z - halfV * bitangent.z
            ),
            XMFLOAT4(Colors::White),
            normal,
            XMFLOAT2(0, 0)
        ));
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(
                center.x - halfU * tangent.x + halfV * bitangent.x,
                center.y - halfU * tangent.y + halfV * bitangent.y,
                center.z - halfU * tangent.z + halfV * bitangent.z
            ),
            XMFLOAT4(Colors::Bisque),
            normal,
            XMFLOAT2(0, 1)
        ));
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(
                center.x + halfU * tangent.x - halfV * bitangent.x,
                center.y + halfU * tangent.y - halfV * bitangent.y,
                center.z + halfU * tangent.z - halfV * bitangent.z
            ),
            XMFLOAT4(Colors::Aquamarine),
            normal,
            XMFLOAT2(1, 0)
        ));
        meshRenderingData.vertexData.push_back(FVertex(
            XMFLOAT3(
                center.x + halfU * tangent.x + halfV * bitangent.x,
                center.y + halfU * tangent.y + halfV * bitangent.y,
                center.z + halfU * tangent.z + halfV * bitangent.z
            ),
            XMFLOAT4(Colors::Red),
            normal,
            XMFLOAT2(1, 1)
            ));
        meshRenderingData.indexData.push_back(0 + i * 4);meshRenderingData.indexData.push_back(2 + i * 4);meshRenderingData.indexData.push_back(3 + i * 4);
        meshRenderingData.indexData.push_back(0 + i * 4);meshRenderingData.indexData.push_back(3 + i * 4);meshRenderingData.indexData.push_back(1 + i * 4);
    }
}