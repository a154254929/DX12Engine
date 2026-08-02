#pragma once
#include <iostream>

struct FFBXRenderData
{
        //index
        //点位置
        //法线
        //切线
        //纹理坐标
};

struct FFBXVersion
{
    int major;
    int minor;
    int revision;
};

struct FFBXAssetImport
{
   void  LoadMeshData(const std::string& inPath, FFBXRenderData& outRenderData);
};