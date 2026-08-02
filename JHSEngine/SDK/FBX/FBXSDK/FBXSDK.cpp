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

bool LoadScene(FbxManager*& inManager, FbxScene*& inScene, const char* inFilename)
{
    FFBXVersion sdkVersion;
    FbxManager::GetFileFormatVersion(
        sdkVersion.major,
        sdkVersion.minor,
        sdkVersion.revision
    );
    
    FbxImporter* fbxImporterPtr = FbxImporter::Create(inManager, "");
    bool bReturn = fbxImporterPtr->Initialize(inFilename, -1, inManager->GetIOSettings());
    
    FFBXVersion filerVersion;
    fbxImporterPtr->GetFileVersion(
        sdkVersion.major,
        sdkVersion.minor,
        sdkVersion.revision
    );
    
    if (!bReturn)
    {
        return false;
    }
    
    if (fbxImporterPtr->IsFBX())
    {
        //打印数据
    }

    bReturn = fbxImporterPtr->Import(inScene);
    
    if (bReturn
        && fbxImporterPtr->GetStatus().GetCode() == FbxStatus::ePasswordError
    )
    {
        //密码
    }
    
    fbxImporterPtr->Destroy();
    
    return true;
}

void RecursiveLoadMesh(FbxNode* inNode, FFBXRenderData& outRenderData)
{
    
}

void FFBXAssetImport::LoadMeshData(const std::string& inPath, FFBXRenderData& outRenderData)
{
    FbxManager* manager = NULL;
    FbxScene* scene = NULL;
    
    InitializeSdkObjects(manager, scene);
    
    FbxString fbxPath(inPath.c_str());
    bool result = LoadScene(manager, scene, fbxPath.Buffer());
    
    //类似XML
    if (FbxNode* rootNode = scene->GetRootNode())
    {
        for (int i = 0; i < rootNode->GetChildCount(); i++)
        {
            RecursiveLoadMesh(rootNode->GetChild(i), outRenderData);
        }
    }
}

int main()
{
    FbxManager* manager = FbxManager::Create();
    
    return 0;
}
