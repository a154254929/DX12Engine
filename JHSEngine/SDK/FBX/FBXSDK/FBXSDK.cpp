#include "FBXSDK.h"
#include "include/fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libfbxsdk-mt.lib")

void InitializeSdkObjects(FbxManager*& inManager, FbxScene*& inScene)
{
    inManager = FbxManager::Create();
    
    FbxIOSettings* fbxIO = FbxIOSettings::Create(inManager, IOSROOT);
    inManager->SetIOSettings(fbxIO);
    
    FbxString fbxPath = FbxGetApplicationDirectory();
    inManager->LoadPluginsDirectory(fbxPath);
    
    inScene = FbxScene::Create(inManager, "My Scene");
}

void FFBXAssetImport::LoadMeshData(const std::string& inPath, FFBXRenderData& outRenderData)
{
    FbxManager* manager = NULL;
    FbxScene* scene = NULL;
    
    InitializeSdkObjects(manager, scene);
}

int main()
{
    FbxManager* manager = FbxManager::Create();
    
    return 0;
}
