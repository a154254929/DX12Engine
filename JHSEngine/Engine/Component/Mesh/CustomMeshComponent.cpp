#include "CustomMeshComponent.h"

#include "../../Mesh/Core/MeshType.h"
#if THIRD_PARTY_LIBRARY

#include "../../../SDK/FBX/FBXSDK/FBXSDK.h"

//#pragma comment(lib, "FBXSDK.lib")

#endif
#include <limits>

CCustomMeshComponent::CCustomMeshComponent()
{
}

void CCustomMeshComponent::CreateMesh(FMeshRenderingData& meshRenderingData, const string& inPath)
{
    char tmpBuff[1024] = { 0 };
    get_path_clean_filename(tmpBuff, inPath.c_str());
    if (find_string(tmpBuff, ".obj", 0) != -1)
    {
        //拿到文件大小
        unsigned int fileSize = get_file_size_by_filename(inPath.c_str());

        //根据文件大小创建buff
        char* buff = new char[fileSize + 1];
        memset(buff, 0, fileSize + 1);
        //提取buff
        get_file_buf(inPath.c_str(), buff);

        if (!LoadObjFromBuff(buff, fileSize, &meshRenderingData))
        {

        }

        delete buff;
    }
    else if (find_string(tmpBuff, ".fbx", 0) != -1)
    {
        LoadFbxFromBuff(inPath, meshRenderingData);
    }
}

void CCustomMeshComponent::BuildKey(size_t& meshHashKey, const string& inPath)
{
    std::hash<string> stringHash;
    
    meshHashKey = 3;
    meshHashKey += stringHash(inPath);
}

bool CCustomMeshComponent::LoadObjFromBuff(char* buff, uint32_t buffSize, FMeshRenderingData* meshRenderingData)
{
    if (buffSize > 0)
    {
        stringstream buffStream(buff);
        char tmpLine[256] = { 0 };
        string headTmp;
        for (; !buffStream.eof();)
        {
            memset(tmpLine, 0, 256);

            buffStream.getline(tmpLine, 256);
            if (strlen(tmpLine) > 0)
            {
                if (tmpLine[0] == 'v')
                {
                    stringstream lineStream(tmpLine);
                    lineStream >> headTmp;
                    if (tmpLine[1] == 't')
                    {

                    }
                    else if (tmpLine[1] == 'n')
                    {

                    }
                    else
                    {
                        //先添加一个点
                        meshRenderingData->vertexData.push_back(FVertex(
                            XMFLOAT3(),
                            XMFLOAT4(Colors::White)
                        ));

                        //拿到添加后的位置
                        int topIndex = meshRenderingData->vertexData.size() - 1;
                        XMFLOAT3& float3InPos = meshRenderingData->vertexData[topIndex].position;

                        lineStream >> float3InPos.x;
                        lineStream >> float3InPos.y;
                        lineStream >> float3InPos.z;
                    }
                }
                else if (tmpLine[0] == 'f')
                {
                    stringstream lineStream(tmpLine);
                    lineStream >> headTmp;
                    char saveLineString[256] = { 0 };
                    char tmpBuff[256] = { 0 };
                    for (int i = 0; i < 3; ++i)
                    {
                        memset(saveLineString, 0, 256);

                        //输入一项数据
                        lineStream >> saveLineString;

                        //找到索引位置
                        int stringPosA = find_string(saveLineString, "/", 0);
                        memset(tmpBuff, 0, 256);

                        //找到位置索引
                        char* vPosIndex = string_mid(saveLineString, tmpBuff, 0, stringPosA);
                        meshRenderingData->indexData.push_back(atoi(vPosIndex) - 1);

                        //找到纹理索引
                        int stringPosB = find_string(saveLineString, "/", stringPosA + 1);
                        memset(tmpBuff, 0, 256);
                        char* vTextoordIndex = string_mid(saveLineString, tmpBuff, stringPosA + 1, stringPosB);

                        //找到纹理索引
                        memset(tmpBuff, 0, 256);
                        char* vNormalIndex = string_mid(saveLineString, tmpBuff, stringPosB + 1, strlen(saveLineString) - (stringPosB + 1));
                    }
                }
            }
        }
    }
    return false;
}

bool CCustomMeshComponent::LoadFbxFromBuff(const string& inPath, FMeshRenderingData& meshData)
{
#if THIRD_PARTY_LIBRARY
    FFBXRenderData renderData;
    FFBXAssetImport().LoadMeshData(inPath, renderData);
    
    for ( auto &tmpModel : renderData.modelData)
    {
        for (auto &tmpMesh : tmpModel.meshData)
        {
            const size_t vertexOffset = meshData.vertexData.size();
            for (auto &tmpTriangle : tmpMesh.vertexData)
            {
                for (int i = 0; i < 3; ++i)
                {
                    FVertex newVertex(
                        XMFLOAT3(tmpTriangle.vertex[i].position.x, tmpTriangle.vertex[i].position.y, tmpTriangle.vertex[i].position.z),
                        XMFLOAT4(tmpTriangle.vertex[i].color.x, tmpTriangle.vertex[i].color.y, tmpTriangle.vertex[i].color.z, 1.f),
                        XMFLOAT3(tmpTriangle.vertex[i].normal.x, tmpTriangle.vertex[i].normal.y, tmpTriangle.vertex[i].normal.z),
                        XMFLOAT2(tmpTriangle.vertex[i].texcoord.x, tmpTriangle.vertex[i].texcoord.y)
                    );
                    meshData.vertexData.push_back(newVertex);
                }
            }

            for (uint16_t index : tmpMesh.indexData)
            {
                const size_t remappedIndex = vertexOffset + index;
                if (remappedIndex <= (std::numeric_limits<uint16_t>::max)())
                {
                    meshData.indexData.push_back(static_cast<uint16_t>(remappedIndex));
                }
            }
        }
        for (auto &tmpMesh : tmpModel.materialMap)
        {
            
        }
    }
#endif
    return false;
}
