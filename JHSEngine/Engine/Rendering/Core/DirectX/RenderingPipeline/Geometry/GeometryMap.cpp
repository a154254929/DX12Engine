#include "GeometryMap.h"

#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../../../Component/Light/Core/LightType.h"
#include "../../../../../Manager/LightManager.h"
#include "../../../../../Component/Light/PointLightComponent.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../RenderingTextureResourceUpdate.h"
#include "../../../../../Component/Sky/FogComponent.h"
#include "../../../../../Component/Sky/Core/FogConstantBuffer.h"
#include "../RenderLayer/RenderLayerManager.h"

UINT meshCount = 0;

FGeometryMap::FGeometryMap()
{
    geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
    
    renderingTexture2DResourcesUpdate = std::make_shared<FRenderingTextureResourcesUpdate>();
    renderingTexture2DResourcesUpdate->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);
    renderingTextureCubemapResourcesUpdate = std::make_shared<FRenderingTextureResourcesUpdate>();
    renderingTextureCubemapResourcesUpdate->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);
    
    fogComponent = NULL;
}

FGeometryMap::~FGeometryMap()
{
    meshCount = 0;
}

void FGeometryMap::PreDraw(float deltaTime)
{
    descriptorHeap.PreDraw(deltaTime);
}

void FGeometryMap::Draw(float deltaTime)
{
    //渲染视口
    //DrawViewport(deltaTime);

    //渲染光源
    DrawLight(deltaTime);
    
    //绘制贴图
    DrawTexture2D(deltaTime);
    
    //绘制贴图
    DrawTextureCubemap(deltaTime);

    //绘制材质
    DrawMaterial(deltaTime);

    //渲染模型
    DrawMesh(deltaTime);
    
    DrawFog(deltaTime);
}

void FGeometryMap::PostDraw(float deltaTime)
{

}

void FGeometryMap::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
    // DEBUG: 打印相机位置和视图矩阵
    /*
    static int frameCount = 0;
    if (frameCount++ % 60 == 0) {  // 每60帧打印一次
        Engine_Log("Camera pos: (%.2f, %.2f, %.2f)", 
            viewportInfo.viewWorldPosition.x, 
            viewportInfo.viewWorldPosition.y, 
            viewportInfo.viewWorldPosition.z);
        Engine_Log("ViewMatrix[3]: (%.2f, %.2f, %.2f, %.2f)", 
            viewportInfo.viewMatrix._41, 
            viewportInfo.viewMatrix._42, 
            viewportInfo.viewMatrix._43,
            viewportInfo.viewMatrix._44);
    }
    */
    
    UpdateMaterialShaderResourceView(deltaTime, viewportInfo);
    
    //更新灯光
    FLightConstantBuffer lightConstantBuffer;
    lightConstantBuffer.lightInfo.x = GetLightManager()->lights.size();
    for (int i = 0; i < GetLightManager()->lights.size(); ++i)
    {
        if (auto light = GetLightManager()->lights[i])
        {
            lightConstantBuffer.sceneLights[i].lightDirection = light->GetForwardVector();
            fvector_3d lightIntensity = light->GetLightIntensity();
            lightConstantBuffer.sceneLights[i].lightIntensity = XMFLOAT3(lightIntensity.x, lightIntensity.y, lightIntensity.z);
            lightConstantBuffer.sceneLights[i].lightPosition = light->GetPosition();
            ELightType lightType = light->GetLightType();
            lightConstantBuffer.sceneLights[i].lightType = (int)lightType;
            switch (lightType)
            {
                case ELightType::PointLight:
                case ELightType::SpotLight:
                    if (CRangeLightComponent* rangeLight = dynamic_cast<CRangeLightComponent*>(light))
                    {
                        lightConstantBuffer.sceneLights[i].startAttenuation = rangeLight->GetStartAttenuation();
                        lightConstantBuffer.sceneLights[i].endAttenuation = rangeLight->GetEndAttenuation();
                        if (lightType == ELightType::SpotLight)
                        {
                            if(CSpotLightComponent* spotLight = dynamic_cast<CSpotLightComponent*>(rangeLight))
                            {
                               lightConstantBuffer.sceneLights[i].startAttenuation = spotLight->GetStartAttenuation();
                               lightConstantBuffer.sceneLights[i].endAttenuation = spotLight->GetEndAttenuation();
                               float angle2Radian = acos(-1.0) / 180.f;
                               lightConstantBuffer.sceneLights[i].conicalInnerCorner = math_utils::angle_to_radian(spotLight->GetConicalInnerCorner());
                               lightConstantBuffer.sceneLights[i].conicalOuterCorner = spotLight->GetConicalOuterCorner() * angle2Radian;
                            }
                        }
                    }
                    break;
            }
        }
    }

    lightConstantBufferView.Update(0, &lightConstantBuffer);

    //更新视口
    UpdateCalculationsViewport(deltaTime, viewportInfo, 0);
    
    //更新雾效
    if (fogComponent)
    {
        FFogConstantBuffer fogConstantBuffer;
        {
            fvector_color fogColor = fogComponent->GetFogColor();
            fogConstantBuffer.fogColor = XMFLOAT3(fogColor.r, fogColor.g, fogColor.b);
            fogConstantBuffer.fogStart = fogComponent->GetFogStart();
            fogConstantBuffer.fogRange = fogComponent->GetFogRange();
            fogConstantBuffer.fogHeight = fogComponent->GetFogHeight();
            fogConstantBuffer.fogTransparentCoefficient = fogComponent->GetFogTransparentCoefficient();
        }
        fogConstantBufferView.Update(0, &fogConstantBuffer);
    }
}

void FGeometryMap::UpdateCalculationsViewport(
    float deltaTime,
    const FViewportInfo viewportInfo,
    UINT inConstantBufferOffset
)
{
    XMMATRIX projectMatrix = XMLoadFloat4x4(&viewportInfo.projectMatrix);
    XMMATRIX viewMatrix = XMLoadFloat4x4(&viewportInfo.viewMatrix);
    XMMATRIX viewProjMatrix = XMMatrixMultiply(viewMatrix, projectMatrix);

    FViewportTransformation viewportTransformation;
    XMStoreFloat4x4(&viewportTransformation.viewProjectionMatrix, XMMatrixTranspose(viewProjMatrix));
    //拿到视口位置
    viewportTransformation.viewportWorldPosirion = viewportInfo.viewWorldPosition;
    viewportConstantBufferView.Update(inConstantBufferOffset, &viewportTransformation);
}

void FGeometryMap::UpdateMaterialShaderResourceView(float deltaTime, const FViewportInfo viewportInfo)
{
    for (size_t i = 0; i < materials.size(); ++i)
    {
        if (CMaterial* inMaterial = materials[i])
        {
            FMaterialConstantBuffer materialConstantBuffer;
            if (inMaterial->IsDirty())
            {
                fvector_4d baseColor = inMaterial->GetBaseColor();
                fvector_3d specularColor = inMaterial->GetSpecularColor();
                materialConstantBuffer.baseColor = XMFLOAT4(baseColor.x, baseColor.y, baseColor.z, baseColor.w);
                materialConstantBuffer.specularColor = XMFLOAT3(specularColor.x, specularColor.y, specularColor.z);

                materialConstantBuffer.roughness = inMaterial->GetRoughness();
                
                fvector_3d fresnelF0 = inMaterial->GetFresenlF0();
                materialConstantBuffer.fresnelF0 = XMFLOAT3(fresnelF0.x, fresnelF0.y, fresnelF0.z);

                materialConstantBuffer.materialType = inMaterial->GetMaterialType();
                        
                //外部资源导入
                if (auto baseColorTextureResourcePtr = FindRenderingTextureByName(inMaterial->GetBaseColorIndexKey()))
                {
                    materialConstantBuffer.baseColorIndex = baseColorTextureResourcePtr->get()->renderingTextureID;
                }
                else
                {
                    materialConstantBuffer.baseColorIndex = -1;
                }
                
                if (auto normalMapResourcePtr = FindRenderingTextureByName(inMaterial->GetNormalMapIndexKey()))
                {
                    materialConstantBuffer.normalMapIndex = normalMapResourcePtr->get()->renderingTextureID;
                }
                else
                {
                    materialConstantBuffer.normalMapIndex = -1;
                }
                
                if (auto specularMapResourcePtr = FindRenderingTextureByName(inMaterial->GetSpecularMapIndexKey()))
                {
                    materialConstantBuffer.specularMapIndex = specularMapResourcePtr->get()->renderingTextureID;
                }
                else
                {
                    materialConstantBuffer.specularMapIndex = -1;
                }
                
                if (auto roughnessMapResourcePtr = FindRenderingTextureByName(inMaterial->GetSpecularMapIndexKey()))
                {
                    materialConstantBuffer.roughnessMapIndex = roughnessMapResourcePtr->get()->renderingTextureID;
                }
                else
                {
                    materialConstantBuffer.roughnessMapIndex = -1;
                }
                    
                XMMATRIX materialTransform = XMLoadFloat4x4(&inMaterial->GetMaterialTransform());
                XMStoreFloat4x4(
                    &materialConstantBuffer.transformation,
                    XMMatrixTranspose(materialTransform)
                );
                inMaterial->SetDirty(false);
            }
            materialConstantBufferView.Update(inMaterial->GetMaterialIndex(), &materialConstantBuffer);
        }
    }
}

void FGeometryMap::BuildMesh(const size_t inMeshHash, CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData)
{
    for (auto& tmp : geometrys)
    {
        tmp.second.BuildMesh(inMeshHash, inMeshComponent, inMeshData, tmp.first);
    }
}

void FGeometryMap::BuildDynamicReflectionMesh()
{
    for (auto& tmp : gObjects)
    {
        if (CMeshComponent* meshComp = dynamic_cast<CMeshComponent*>(tmp))
        {
            if (meshComp->IsDynamicReflection())
            {
                dynamicReflectionMeshComponents.push_back(meshComp);
            }
        }
    }
}

void FGeometryMap::DuplicateMesh(CMeshComponent* inMeshComponent, const FRenderingData& meshRenderingData)
{
    for (auto& tmp : geometrys)
    {
        tmp.second.DuplicateMesh(inMeshComponent, meshRenderingData, tmp.first);
    }
}

bool FGeometryMap::FindMeshRenderingData(const size_t& inHash, FRenderingData& meshData, int inRenderLayerType)
{
    for (auto &tmp : geometrys)
    {
        if (tmp.second.FindMeshRenderingData(inHash, meshData, inRenderLayerType))
        {
            return true;
        }
    }
    return false;
}

void FGeometryMap::Build()
{
    //构建模型
    for (auto& tmp : geometrys)
    {
        tmp.second.Build();
    }
}

void FGeometryMap::BuildDescriptorHeap()
{
    //+1是因为有摄像机
    descriptorHeap.Build(
        GetDrawTexture2DResourcesNumber()           //Texture2D
        + GetDrawTextureCubemapResourcesNumber()    //静态Cubemap
        + 1                                         //动态Cubmap
    );
}

void FGeometryMap::BuildMeshConstantBuffer()
{
    //创建常量缓冲区
    meshConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectNumber());

    //CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    //meshConstantBufferView.BuildConstantBuffer(desHandle, GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
    //收集材质,真正收集shader-Index
    int shaderIndex = 0;
    for (auto& renderLayer : FRenderLayerManager::renderLayers)
    {
        for (auto& renderingData : renderLayer->renderingDatas)
        {
            if (auto meshCompMaterials = renderingData.meshComp->GetMaterials())
            {
                for (int j = 0; j < meshCompMaterials->size(); ++j)
                {
                    (*meshCompMaterials)[j]->SetMaterialIndex(shaderIndex);
                    materials.push_back((*meshCompMaterials)[j]);
                    shaderIndex++;
                }
            }
        }
    }
    
    //创建常量缓冲区
    materialConstantBufferView.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialObjectNumber(), false);
}

void FGeometryMap::BuildLightConstantBuffer()
{
    //创建常量缓冲区
    lightConstantBufferView.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

    //CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    //lightConstantBufferView.BuildConstantBuffer(desHandle, GetDrawLightObjectNumber(), GetDrawMeshObjectNumber() );
}

void FGeometryMap::BuildFogConstantBuffer()
{
    fogConstantBufferView.CreateConstant(sizeof(FFogConstantBuffer), 1);
}

void FGeometryMap::BuildViewportConstantBuffer(UINT inViewportOffset)
{
    //创建常量缓冲区
    viewportConstantBufferView.CreateConstant(
        sizeof(FViewportTransformation),
        1
        + GetDynamicReflectionMeshObjectNumber() * 6
        + inViewportOffset
    );

    //CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    //viewportConstantBufferView.BuildConstantBuffer(desHandle, 1, GetDrawMeshObjectNumber() + GetDrawLightObjectNumber());
}

void FGeometryMap::BuildTextureConstantBuffer()
{
    //构建Texture2D
    renderingTexture2DResourcesUpdate->BuildTextureConstantBuffer(
        descriptorHeap.GetHeap(),
        0
    );
    //构建TextureCubemap
    renderingTextureCubemapResourcesUpdate->BuildTextureConstantBuffer(
        descriptorHeap.GetHeap(),
        GetDrawTexture2DResourcesNumber()
    );
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
    return geometrys[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
    return materials.size();
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
    return 1;
}

UINT FGeometryMap::GetDrawTexture2DResourcesNumber()
{
    return renderingTexture2DResourcesUpdate->Size();
}

UINT FGeometryMap::GetDrawTextureCubemapResourcesNumber()
{
    return renderingTextureCubemapResourcesUpdate->Size();
}

UINT FGeometryMap::GetDynamicReflectionMeshObjectNumber()
{
    return dynamicReflectionMeshComponents.size();
}

std::unique_ptr<FRenderingTexture>* FGeometryMap::FindRenderingTextureByName(const std::string& inKey)
{
    if (auto texture2DPtr = renderingTexture2DResourcesUpdate->FindRenderingTextureByName(inKey))
    {
        return texture2DPtr;
    }
    else if (auto textureCubemapPtr = renderingTextureCubemapResourcesUpdate->FindRenderingTextureByName(inKey))
    {
        return textureCubemapPtr;
    }
    return nullptr;
}

void FGeometryMap::DrawViewport(float deltaTime)
{
    // UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
    //
    // desHandle.Offset(GetDrawMeshObjectNumber() + GetDrawLightObjectNumber(), descriptorOffset);
    // GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, desHandle);
    
    GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
        1,
        viewportConstantBufferView.GetBuffer()->GetGPUVirtualAddress()
    );
}

void FGeometryMap::DrawLight(float deltaTime)
{
    // UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
    //
    // desHandle.Offset(GetDrawMeshObjectNumber(), descriptorOffset);
    // GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, desHandle);
        
    GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
        2,
        lightConstantBufferView.GetBuffer()->GetGPUVirtualAddress()
    );
}

void FGeometryMap::DrawMesh(float deltaTime)
{
    
}

void FGeometryMap::DrawFog(float deltaTime)
{
        
    GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
        3,
        fogConstantBufferView.GetBuffer()->GetGPUVirtualAddress()
    );
}

void FGeometryMap::DrawMaterial(float deltaTime)
{
   
    GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(
        4,
        materialConstantBufferView.GetBuffer()->GetGPUVirtualAddress()
    );
}

void FGeometryMap::DrawTexture2D(float deltaTime)
{
    UINT descriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
    {
        CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

        desHandle.Offset(
            0,
            descriptorOffset
        );
        GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, desHandle);
    }
}

void FGeometryMap::DrawTextureCubemap(float deltaTime)
{
    UINT descriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
    {
        CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

        desHandle.Offset(
            GetDrawTexture2DResourcesNumber(),
            descriptorOffset
        );
        GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, desHandle);
    }
}

void FGeometryMap::LoadTexture()
{
    def_c_paths filePaths;
    init_def_c_paths(&filePaths);
    
    char rootPath[] = "../JHSEngine/Asset/Texture";
    find_files(rootPath, &filePaths, true);

    std::vector<std::string> ddsPaths;
    ddsPaths.reserve(filePaths.index);

    for (int i = 0; i < filePaths.index; ++i)
    {
        if (find_string(filePaths.paths[i], ".dds", 0) != -1)
        {
            normalization_path(filePaths.paths[i]);
            ddsPaths.emplace_back(filePaths.paths[i]);
        }
    }

    auto getFileName = [](const std::string& p) -> std::string
    {
        size_t pos = p.find_last_of("/\\");
        return (pos == std::string::npos) ? p : p.substr(pos + 1);
    };

    auto toLower = [](std::string s) -> std::string
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    };

    // 先按文件名排序；文件名相同再按完整路径排序，保证结果稳定
    std::sort(ddsPaths.begin(), ddsPaths.end(),
        [&](const std::string& a, const std::string& b)
        {
            std::string an = toLower(getFileName(a));
            std::string bn = toLower(getFileName(b));
            if (an != bn) return an < bn;
            return toLower(a) < toLower(b);
        });

    for (const std::string& p : ddsPaths)
    {
        wchar_t texturePath[1024] = { 0 };
        char_to_wchar_t(texturePath, 1024, p.c_str());
        if (wfind_string(texturePath, L"_CubeMap.") != -1 || wfind_string(texturePath, L"_cubemap.") != -1)
        {
            renderingTextureCubemapResourcesUpdate->LoadTextureResource(texturePath);
        }
        else
        {
            renderingTexture2DResourcesUpdate->LoadTextureResource(texturePath);
        }
    }
}

void FGeometryMap::BuildFog()
{
    for (auto& object : gObjects)
    {
        if (CFogComponent* fogComp = dynamic_cast<CFogComponent*>(object))
        {
            fogComponent = fogComp;
            break;
        }
    }
}

bool FGeometryMap::IsFogOn()
{
    return fogComponent != NULL;
}

bool FGeometry::IsRenderingDataExistence(CMeshComponent* inKey)
{
    if (std::shared_ptr<FRenderLayer> renderLayer = FRenderLayerManager::FindByRenderLayer((int)inKey->GetRenderLayerType()))
    {
        for (auto& tmp : renderLayer->renderingDatas)
        {
            if (tmp.meshComp == inKey)
                return true;
        }
    }
    return false;
}

void FGeometry::BuildMesh(
    const size_t inMeshHash,
    CMeshComponent* inMeshComponent,
    const FMeshRenderingData& inMeshData,
    int inGeometryKey
)
{
    if (!IsRenderingDataExistence(inMeshComponent))
    {
        //找到对于层级
        if (auto renderLayer = FRenderLayerManager::FindByRenderLayer((int)inMeshComponent->GetRenderLayerType()))
        {
            renderLayer->renderingDatas.push_back(FRenderingData());
            FRenderingData& inRenderingData = renderLayer->renderingDatas[renderLayer->renderingDatas.size() - 1];

            //基础信息记录
            inRenderingData.meshComp = inMeshComponent;
            inRenderingData.meshHash = inMeshHash;
            inRenderingData.meshObjectIndex = meshCount++;
            inRenderingData.geometryKey = inGeometryKey;
            inRenderingData.indexSize = inMeshData.indexData.size();
            inRenderingData.vertexSize = inMeshData.vertexData.size();

            inRenderingData.indexOffsetPosition = meshRenderingData.indexData.size();
            inRenderingData.vertexOffsetPosition = meshRenderingData.vertexData.size();

            //索引的合并
            meshRenderingData.indexData.insert(
                meshRenderingData.indexData.end(),
                inMeshData.indexData.begin(),
                inMeshData.indexData.end()
            );
            //顶点的合并
            meshRenderingData.vertexData.insert(
                meshRenderingData.vertexData.end(),
                inMeshData.vertexData.begin(),
                inMeshData.vertexData.end()
            );
        }
    }
}

void FGeometry::DuplicateMesh(
    CMeshComponent* inMeshComponent,
    const FRenderingData& meshRenderingData,
    int inGeometryKey
)
{
    if (!IsRenderingDataExistence(inMeshComponent))
    {
        if (auto renderLayer = FRenderLayerManager::FindByRenderLayer((int)inMeshComponent->GetRenderLayerType()))
        {
            renderLayer->renderingDatas.push_back(meshRenderingData);
            FRenderingData& inRenderingData = renderLayer->renderingDatas[renderLayer->renderingDatas.size() - 1];

            //基础信息记录
            inRenderingData.meshComp = inMeshComponent;
            inRenderingData.meshObjectIndex = meshCount++;
            inRenderingData.geometryKey = inGeometryKey;
        }
    }
}

bool FGeometry::FindMeshRenderingData(const size_t& inHash, FRenderingData& meshData, int inRenderLayerType)
{
    //寻找RenderingData
    auto FindMeshRenderDataByHash = [&](std::shared_ptr<FRenderLayer> inRenderingLayer ) -> FRenderingData*
    {
        for (auto& tmp : inRenderingLayer->renderingDatas)
        {
            if (tmp.meshHash == inHash)
            {
                return &tmp;
            }
        }
        return NULL;
    };
    //暴力寻找
    if (inRenderLayerType == -1)
    {
        for (auto& tmpRenderLayer : FRenderLayerManager::renderLayers)
        {
            if (FRenderingData* outRenderingData = FindMeshRenderDataByHash(tmpRenderLayer))
            {
                meshData = *outRenderingData;
                return true;
            }
        }
    }
    //精准寻找
    else if (auto renderLayer = FRenderLayerManager::FindByRenderLayer(inRenderLayerType))
    {
        if (FRenderingData* outRenderingData = FindMeshRenderDataByHash(renderLayer))
        {
            meshData = *outRenderingData;
            return true;
        }
    }
    return false;
}

void FGeometry::Build()
{
    //构建模型
    UINT vertexSizeInBytes = meshRenderingData.GetVertexSizeInBytes();
    UINT indexSizeInBytes = meshRenderingData.GetIndexSizeInBytes();

    ANALYSIS_HRESULT(D3DCreateBlob(
        vertexSizeInBytes,
        &cpuVertexBufferPtr
    ));
    memcpy(
        cpuVertexBufferPtr->GetBufferPointer(),
        meshRenderingData.vertexData.data(),
        vertexSizeInBytes
    );

    ANALYSIS_HRESULT(D3DCreateBlob(
        indexSizeInBytes,
        &cpuIndexBufferPtr
    ));
    memcpy(
        cpuIndexBufferPtr->GetBufferPointer(),
        meshRenderingData.indexData.data(),
        indexSizeInBytes
    );

    ConstructBuffer::FConstructBuffer constructBuffer;
    gpuVertexBufferPtr = constructBuffer.ConstructDefaultBuffer(
        vertexBufferTmpPtr,
        meshRenderingData.vertexData.data(),
        vertexSizeInBytes
    );
    gpuIndexBufferPtr = constructBuffer.ConstructDefaultBuffer(
        indexBufferTmpPtr,
        meshRenderingData.indexData.data(),
        indexSizeInBytes
    );
}

UINT FGeometry::GetDrawObjectNumber() const
{
    int objCount = 0;
    for (auto& tmpRenderLayer : FRenderLayerManager::renderLayers)
    {
        for (auto& tmp : tmpRenderLayer->renderingDatas)
        {
            objCount++;
        }
    }
    return objCount;
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW vbv;
    vbv.BufferLocation = gpuVertexBufferPtr->GetGPUVirtualAddress();
    vbv.SizeInBytes = meshRenderingData.GetVertexSizeInBytes();
    vbv.StrideInBytes = sizeof(FVertex);
    return vbv;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW ibv;
    ibv.BufferLocation = gpuIndexBufferPtr->GetGPUVirtualAddress();
    ibv.SizeInBytes = meshRenderingData.GetIndexSizeInBytes();
    ibv.Format = DXGI_FORMAT_R16_UINT;
    return ibv;
}
