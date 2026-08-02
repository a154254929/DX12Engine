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
        filerVersion.major,
        filerVersion.minor,
        filerVersion.revision
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

void GetPolygons(FbxMesh*& inMesh, FFBXRenderData& outRenderData)
{
    if (inMesh == NULL)
    {
        return;
    }
    
    int polygonCount = inMesh->GetPolygonCount();
    FbxVector4* controlPoints = inMesh->GetControlPoints();
    
    int vertexId = 0;
    
    for (int i = 0; i < polygonCount; ++i)
    {
        int polygonSize = inMesh->GetPolygonSize(i);
        for (int j = 0; j < polygonSize; ++j)
        {
            int controlPointIndex = inMesh->GetPolygonVertex(i, j);
            FbxVector4 controlPoint = inMesh->GetControlPointAt(controlPointIndex);
            
            //UV
            for (int l = 0; l < inMesh->GetElementUVCount(); ++l)
            {
                FbxGeometryElementUV* textureUV = inMesh->GetElementUV(l);
                auto mappingMode = textureUV->GetMappingMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    int textureUVIndex = inMesh->GetTextureUVIndex(i, j);
                    auto referenceMode = textureUV->GetReferenceMode();
                    
                    if (referenceMode == FbxLayerElement::eIndex
                        || referenceMode == FbxLayerElement::eIndexToDirect
                    )
                    {
                        int index = textureUV->GetIndexArray().GetAt(textureUVIndex);
                        FbxVector2 uv = textureUV->GetDirectArray().GetAt(index);
                    }
                    else if (referenceMode == FbxLayerElement::eDirect)
                    {
                        FbxVector2 uv = textureUV->GetDirectArray().GetAt(textureUVIndex);
                    }
                }
            }
            
            //法线
            for (int l = 0; l < inMesh->GetElementNormalCount(); ++l)
            {
                FbxGeometryElementNormal* meshNormal = inMesh->GetElementNormal(l);
                auto mappingMode = meshNormal->GetMappingMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    auto referenceMode = meshNormal->GetReferenceMode();
                    
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 normal = meshNormal->GetDirectArray().GetAt(vertexId);
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshNormal->GetIndexArray().GetAt(vertexId);
                            FbxVector4 tangent = meshNormal->GetDirectArray().GetAt(id);
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
            }
            
            //切线
            for (int l = 0; l < inMesh->GetElementTangentCount(); ++l)
            {
                FbxGeometryElementTangent* meshTangent = inMesh->GetElementTangent(l);
                auto mappingMode = meshTangent->GetMappingMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    auto referenceMode = meshTangent->GetReferenceMode();
                    
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 normal = meshTangent->GetDirectArray().GetAt(vertexId);
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshTangent->GetIndexArray().GetAt(vertexId);
                            FbxVector4 tangent = meshTangent->GetDirectArray().GetAt(id);
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
            }
            for (int l = 0; l < inMesh->GetElementBinormalCount(); ++l)
            {
                FbxGeometryElementBinormal* meshBinormal = inMesh->GetElementBinormal(l);
                auto mappingMode = meshBinormal->GetMappingMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    auto referenceMode = meshBinormal->GetReferenceMode();
                    
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 binormal = meshBinormal->GetDirectArray().GetAt(vertexId);
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshBinormal->GetIndexArray().GetAt(vertexId);
                            FbxVector4 binormal = meshBinormal->GetDirectArray().GetAt(id);
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
            }
            vertexId++;
        }
    }
}

void GetMesh(FbxNode*& inNode, FFBXRenderData& outRenderData)
{
    FbxMesh* nodeMesh = (FbxMesh*)inNode->GetNodeAttribute();
    
    GetPolygons(nodeMesh, outRenderData);
}

void RecursiveLoadMesh(FbxNode*& inNode, FFBXRenderData& outRenderData)
{
    if (inNode->GetNodeAttribute() == NULL)
    {
        //NULL Node
    }
    else
    {
        FbxNodeAttribute::EType attributeType = inNode->GetNodeAttribute()->GetAttributeType();
        if (attributeType == FbxNodeAttribute::eMesh)
        {
            GetMesh(inNode, outRenderData);
        }
        else if (attributeType == FbxNodeAttribute::eSkeleton)
        {
            
        }
    }
    
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
            FbxNode* child = rootNode->GetChild(i);
            RecursiveLoadMesh(child, outRenderData);
        }
    }
}

int main()
{
    std::string fbxPath = "D:/JHSEngine/JHSEngine/JHSEngine/SDK/FBX/FBXSDK/Pet_Huqiu.FBX";
    FFBXRenderData outRenderData;
    FFBXAssetImport fbxImporter = FFBXAssetImport(); 
    fbxImporter.LoadMeshData(fbxPath, outRenderData);
    
    return 0;
}
