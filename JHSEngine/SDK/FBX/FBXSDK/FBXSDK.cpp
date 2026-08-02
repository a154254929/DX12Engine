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

void GetPolygons(FbxMesh*& inMesh, FFBXMesh& outFBXMesh)
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
        FFBXTriangle newTriangle = FFBXTriangle();
        outFBXMesh.vertexData.push_back(newTriangle);
        int polygonSize = inMesh->GetPolygonSize(i);
        for (int j = 0; j < polygonSize; ++j)
        {
            newTriangle.vertex[j] = FFBXVertex();
            FFBXVertex& newVertex = newTriangle.vertex[j];
            
            int controlPointIndex = inMesh->GetPolygonVertex(i, j);
            FbxVector4 controlPoint = inMesh->GetControlPointAt(controlPointIndex);
            newVertex.position.x = controlPoint.mData[0];
            newVertex.position.y = controlPoint.mData[1];
            newVertex.position.z = controlPoint.mData[2];
            
            //UV
            for (int l = 0; l < inMesh->GetElementUVCount(); ++l)
            {
                FbxGeometryElementUV* textureUV = inMesh->GetElementUV(l);
                auto mappingMode = textureUV->GetMappingMode();
                    auto referenceMode = textureUV->GetReferenceMode();
                if (mappingMode == FbxLayerElement::eByControlPoint)
                {
                    int textureUVIndex = inMesh->GetTextureUVIndex(i, j);
                    
                    if (referenceMode == FbxLayerElement::eIndex
                        || referenceMode == FbxLayerElement::eIndexToDirect
                    )
                    {
                        int index = textureUV->GetIndexArray().GetAt(controlPointIndex);
                        FbxVector2 uv = textureUV->GetDirectArray().GetAt(index);
                        newVertex.texcoord.x = uv.mData[0];
                        newVertex.texcoord.y = 1.0 - uv.mData[1];
                    }
                    else if (referenceMode == FbxLayerElement::eDirect)
                    {
                        FbxVector2 uv = textureUV->GetDirectArray().GetAt(controlPointIndex);
                        newVertex.texcoord.x = uv.mData[0];
                        newVertex.texcoord.y = 1.0 - uv.mData[1];
                    }
                }
                else if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    int textureUVIndex = inMesh->GetTextureUVIndex(i, j);
                    
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector2 uv = textureUV->GetDirectArray().GetAt(textureUVIndex);
                            newVertex.texcoord.x = uv.mData[0];
                            newVertex.texcoord.y = 1.0 - uv.mData[1];
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int index = textureUV->GetIndexArray().GetAt(textureUVIndex);
                            FbxVector2 uv = textureUV->GetDirectArray().GetAt(index);
                            newVertex.texcoord.x = uv.mData[0];
                            newVertex.texcoord.y = 1.0 - uv.mData[1];
                            break;
                        }
                    default:
                        break;
                        
                    }
                }
            }
            
            //法线
            for (int l = 0; l < inMesh->GetElementNormalCount(); ++l)
            {
                FbxGeometryElementNormal* meshNormal = inMesh->GetElementNormal(l);
                auto mappingMode = meshNormal->GetMappingMode();
                auto referenceMode = meshNormal->GetReferenceMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 normal = meshNormal->GetDirectArray().GetAt(vertexId);
                            newVertex.normal.x = normal.mData[0];
                            newVertex.normal.y = normal.mData[1];
                            newVertex.normal.z = normal.mData[2];
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshNormal->GetIndexArray().GetAt(vertexId);
                            FbxVector4 normal = meshNormal->GetDirectArray().GetAt(id);
                            newVertex.normal.x = normal.mData[0];
                            newVertex.normal.y = normal.mData[1];
                            newVertex.normal.z = normal.mData[2];
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
                else if (mappingMode == FbxLayerElement::eByControlPoint)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 normal = meshNormal->GetDirectArray().GetAt(controlPointIndex);
                            newVertex.normal.x = normal.mData[0];
                            newVertex.normal.y = normal.mData[1];
                            newVertex.normal.z = normal.mData[2];
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshNormal->GetIndexArray().GetAt(controlPointIndex);
                            FbxVector4 normal = meshNormal->GetDirectArray().GetAt(id);
                            newVertex.normal.x = normal.mData[0];
                            newVertex.normal.y = normal.mData[1];
                            newVertex.normal.z = normal.mData[2];
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
                auto referenceMode = meshTangent->GetReferenceMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 tangent = meshTangent->GetDirectArray().GetAt(vertexId);
                            newVertex.tangent.x = tangent.mData[0];
                            newVertex.tangent.y = tangent.mData[1];
                            newVertex.tangent.z = tangent.mData[2];
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshTangent->GetIndexArray().GetAt(vertexId);
                            FbxVector4 tangent = meshTangent->GetDirectArray().GetAt(id);
                            newVertex.tangent.x = tangent.mData[0];
                            newVertex.tangent.y = tangent.mData[1];
                            newVertex.tangent.z = tangent.mData[2];
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
                else if (mappingMode == FbxLayerElement::eByControlPoint)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 tangent = meshTangent->GetDirectArray().GetAt(controlPointIndex);
                            newVertex.tangent.x = tangent.mData[0];
                            newVertex.tangent.y = tangent.mData[1];
                            newVertex.tangent.z = tangent.mData[2];
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshTangent->GetIndexArray().GetAt(controlPointIndex);
                            FbxVector4 tangent = meshTangent->GetDirectArray().GetAt(id);
                            newVertex.tangent.x = tangent.mData[0];
                            newVertex.tangent.y = tangent.mData[1];
                            newVertex.tangent.z = tangent.mData[2];
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
                auto referenceMode = meshBinormal->GetReferenceMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 binormal = meshBinormal->GetDirectArray().GetAt(vertexId);
                            newVertex.binormal.x = binormal.mData[0];
                            newVertex.binormal.y = binormal.mData[1];
                            newVertex.binormal.z = binormal.mData[2];
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshBinormal->GetIndexArray().GetAt(vertexId);
                            FbxVector4 binormal = meshBinormal->GetDirectArray().GetAt(id);
                            newVertex.binormal.x = binormal.mData[0];
                            newVertex.binormal.y = binormal.mData[1];
                            newVertex.binormal.z = binormal.mData[2];
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
                else if (mappingMode == FbxLayerElement::eByControlPoint)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxVector4 binormal = meshBinormal->GetDirectArray().GetAt(controlPointIndex);
                            newVertex.binormal.x = binormal.mData[0];
                            newVertex.binormal.y = binormal.mData[1];
                            newVertex.binormal.z = binormal.mData[2];
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshBinormal->GetIndexArray().GetAt(controlPointIndex);
                            FbxVector4 binormal = meshBinormal->GetDirectArray().GetAt(id);
                            newVertex.binormal.x = binormal.mData[0];
                            newVertex.binormal.y = binormal.mData[1];
                            newVertex.binormal.z = binormal.mData[2];
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            
            for (int l = 0; l < inMesh->GetElementVertexColorCount(); ++l)
            {
                FbxGeometryElementVertexColor* meshColor = inMesh->GetElementVertexColor(l);
                auto mappingMode = meshColor->GetMappingMode();
                auto referenceMode = meshColor->GetReferenceMode();
                if (mappingMode == FbxLayerElement::eByPolygonVertex)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxColor color = meshColor->GetDirectArray().GetAt(vertexId);
                            newVertex.color.x = color.mRed;
                            newVertex.color.y = color.mGreen;
                            newVertex.color.z = color.mBlue;
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshColor->GetIndexArray().GetAt(vertexId);
                            FbxColor color = meshColor->GetDirectArray().GetAt(id);
                            newVertex.color.x = color.mRed;
                            newVertex.color.y = color.mGreen;
                            newVertex.color.z = color.mBlue;
                            break;
                        }
                        default:
                            break;
                        
                    }
                }
                else if (mappingMode == FbxLayerElement::eByControlPoint)
                {
                    switch (referenceMode)
                    {
                        case FbxLayerElement::eDirect:
                        {
                            FbxColor color = meshColor->GetDirectArray().GetAt(controlPointIndex);
                            newVertex.color.x = color.mRed;
                            newVertex.color.y = color.mGreen;
                            newVertex.color.z = color.mBlue;
                            break;
                        }
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                        {
                            int id = meshColor->GetIndexArray().GetAt(controlPointIndex);
                            FbxColor color = meshColor->GetDirectArray().GetAt(id);
                            newVertex.color.x = color.mRed;
                            newVertex.color.y = color.mGreen;
                            newVertex.color.z = color.mBlue;
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

void GetMaterial();

void GetMesh(FbxNode*& inNode, FFBXModel& outFBXModel)
{
    FbxMesh* nodeMesh = (FbxMesh*)inNode->GetNodeAttribute();
    
    outFBXModel.meshData.push_back(FFBXMesh());
    FFBXMesh& newMesh = outFBXModel.meshData[outFBXModel.meshData.size() - 1];
    GetPolygons(nodeMesh, newMesh);
    
    //GetMaterial();
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
            outRenderData.modelData.push_back(FFBXModel());
            FFBXModel& newModel = outRenderData.modelData[outRenderData.modelData.size() - 1];
            GetMesh(inNode, newModel);
        }
        else if (attributeType == FbxNodeAttribute::eSkeleton)
        {
            
        }
    }
    
}

void DestroyFbxObjects(FbxManager*& inManager)
{
    if (inManager)
    {
        inManager->Destroy();
    }
}

void FFBXAssetImport::LoadMeshData(const std::string& inPath, FFBXRenderData& outRenderData)
{
    //创建基础管理和场景
    FbxManager* manager = NULL;
    FbxScene* scene = NULL;
    
    //初始化场景对象
    InitializeSdkObjects(manager, scene);
    
    //读取fbx模型
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
    
    DestroyFbxObjects(manager);
}

int main()
{
    std::string fbxPath = "D:/JHSEngine/JHSEngine/JHSEngine/SDK/FBX/FBXSDK/Pet_Huqiu.FBX";
    FFBXRenderData outRenderData;
    FFBXAssetImport fbxImporter = FFBXAssetImport(); 
    fbxImporter.LoadMeshData(fbxPath, outRenderData);
    
    return 0;
}
