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

struct FFBXAssetImport
{
   void  LoadMeshData(const std::string& inPath, FFBXRenderData& outRenderData);
};