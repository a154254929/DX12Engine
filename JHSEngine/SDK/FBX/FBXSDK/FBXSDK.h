#pragma once
#include <iostream>
#include <map>
#include <vector>

#ifdef FBXSDK_EXPORTS
#define FBXASSERTIMPORT_API __declspec(dllexport)
#else
#define FBXASSERTIMPORT_API __declspec(dllexport)
#endif

//2D向量
struct FFBXVector2
{
    FFBXVector2()
    {
        x = 0;
        y = 0;
    }
    float x, y;
};

//3D向量
struct FFBXVector3 : public FFBXVector2
{
    FFBXVector3()
        : FFBXVector2()
    {
        z = 0;
    }
    float z;
};

//顶点数据
struct FBXASSERTIMPORT_API FFBXVertex
{
    FFBXVertex()
        : position()
        , color()
        , texcoord()
        , normal()
        , tangent()
        , binormal()
    {
        
    }
    FFBXVector3 position;
    FFBXVector3 color;
    FFBXVector2 texcoord;
    FFBXVector3 normal;
    FFBXVector3 tangent;
    FFBXVector3 binormal;
    
};

struct FBXASSERTIMPORT_API FFBXTriangle
{
    FFBXTriangle()
        : materialId(0)
    {
        
    }
    FFBXVertex vertex[3];
    int materialId;
};

//模型数据
struct FBXASSERTIMPORT_API FFBXMesh
{
    FFBXMesh()
        : materialId(0)
    {
        
    }
    
    std::vector<FFBXTriangle> vertexData;
    std::vector<uint16_t> indexData;
    
    int materialId;
};

//材质名称
struct FBXASSERTIMPORT_API FFBXMaterial
{
    std::string diffuseTextureFileName;
    std::string specularTextureFileName;
    std::string alphaTextureFileName;
    std::string bumpTextureFileName;
};

struct FBXASSERTIMPORT_API FFBXModel
{
    std::vector<FFBXMesh> meshData;
    std::map<int, FFBXMaterial> materialMap;
};

//渲染数据
struct FBXASSERTIMPORT_API FFBXRenderData
{
    std::vector<FFBXModel> modelData;
};

struct FBXASSERTIMPORT_API FFBXVersion
{
    int major;
    int minor;
    int revision;
};

struct FFBXAssetImport
{
   void  LoadMeshData(const std::string& inPath, FFBXRenderData& outRenderData);
};