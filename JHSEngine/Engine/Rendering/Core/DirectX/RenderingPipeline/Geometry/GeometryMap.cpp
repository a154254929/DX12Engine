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

FGeometryMap::FGeometryMap()
{
    geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
    
    renderingTextureResourcesUpdate = std::make_shared<FRenderingTextureResourcesUpdate>();
}

void FGeometryMap::PreDraw(float deltaTime)
{
    descriptorHeap.PreDraw(deltaTime);
}

void FGeometryMap::Draw(float deltaTime)
{
    //渲染视口
    DrawViewport(deltaTime);

    //渲染光源
    DrawLight(deltaTime);
    
    //绘制贴图
    DrawTexture(deltaTime);

    //渲染模型
    DrawMesh(deltaTime);
}

void FGeometryMap::PostDraw(float deltaTime)
{

}

void FGeometryMap::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
    // DEBUG: 打印相机位置和视图矩阵
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
    //XMINT3 cameraPos = XMINT3(viewportInfo.viewMatrix.m[3][0], viewportInfo.viewMatrix.m[3][1], viewportInfo.viewMatrix.m[3][2]);
    //XMINT3 cameraPos = XMINT3(5.0f, 5.0f, 5.0f);

     //XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
     //XMVECTOR viewTarget = XMVectorZero();
     //XMVECTOR viewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

     //XMMATRIX viewLookAt = XMMatrixLookAtLH(pos, viewTarget, viewUp);
     //XMStoreFloat4x4(const_cast<XMFLOAT4X4*>(&viewportInfo.viewMatrix), viewLookAt);
     //XMMATRIX viewLookAt = XMLoadFloat4x4(&viewportInfo.viewMatrix);

     //XMMATRIX artixProject = XMLoadFloat4x4(&viewportInfo.projectMatrix);
     //XMMATRIX wvp = artixWorld * viewLookAt * artixProject;

    //XMMATRIX wrold = XMLoadFloat4x4(&worldMatrix);
    for (int i = 0; i < geometrys.size(); ++i)
    {
        for (size_t j = 0; j < geometrys[i].describeMeshRenderingData.size(); ++j)
        {
            FRenderingData& inRenderingData = geometrys[i].describeMeshRenderingData[j];
            {
                XMFLOAT3& position = inRenderingData.meshComp->GetPosition();
                fvector_3d scale = inRenderingData.meshComp->GetScale();

                XMFLOAT3 rightVector = inRenderingData.meshComp->GetRightVector();
                XMFLOAT3 upVector = inRenderingData.meshComp->GetUpVector();
                XMFLOAT3 forwardVector = inRenderingData.meshComp->GetForwardVector();

                inRenderingData.worldMatrix = {
                    rightVector.x * scale.x,    upVector.x,                forwardVector.x ,            0.f,
                    rightVector.y,                upVector.y * scale.y,    forwardVector.y,            0.f,
                    rightVector.z,                upVector.z ,            forwardVector.z * scale.z,    0.f,
                    position.x,                    position.y,                position.z,                    1.f
                };
            }
            //更新模型位置
            XMMATRIX artixWorld = XMLoadFloat4x4(&inRenderingData.worldMatrix);
            XMMATRIX artixTextureTransfom = XMLoadFloat4x4(&inRenderingData.textureTransform);
            FObjectTransformation objectTransformation;
            XMStoreFloat4x4(&objectTransformation.world, XMMatrixTranspose(artixWorld));
            XMStoreFloat4x4(&objectTransformation.textureTransformation, XMMatrixTranspose(artixTextureTransfom));
            meshConstantBufferView.Update(j, &objectTransformation);

            //变换材质
            FMaterialConstantBuffer materialConstantBuffer;
            {
                if (CMaterial* material = (*(inRenderingData.meshComp->GetMaterials()))[0])
                {
                    fvector_4d baseColor = material->GetBaseColor();
                    materialConstantBuffer.baseColor = XMFLOAT4(baseColor.x, baseColor.y, baseColor.z, baseColor.w);

                    materialConstantBuffer.roughness = material->GetRoughness();

                    materialConstantBuffer.materialType = material->GetMaterialType();
                    
                    XMMATRIX materialTransform = XMLoadFloat4x4(&material->GetMaterialTransform());
                    XMStoreFloat4x4(
                        &materialConstantBuffer.transformation,
                        XMMatrixTranspose(materialTransform)
                    );
                }

            }
            materialConstantBufferView.Update(j, &materialConstantBuffer);
        }
    }
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
    XMMATRIX projectMatrix = XMLoadFloat4x4(&viewportInfo.projectMatrix);
    XMMATRIX viewMatrix = XMLoadFloat4x4(&viewportInfo.viewMatrix);
    XMMATRIX viewProjMatrix = XMMatrixMultiply(viewMatrix, projectMatrix);

    FViewportTransformation viewportTransformation;
    XMStoreFloat4x4(&viewportTransformation.viewProjectionMatrix, XMMatrixTranspose(viewProjMatrix));
    //拿到视口位置
    viewportTransformation.viewportWorldPosirion = viewportInfo.viewWorldPosition;
    viewportConstantBufferView.Update(0, &viewportTransformation);
}

void FGeometryMap::BuildMesh(CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData)
{
    FGeometry& geometry = geometrys[0];
    geometry.BuildMesh(inMeshComponent, inMeshData);
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
    descriptorHeap.Build(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber() + 1 + 1);
}

void FGeometryMap::BuildMeshConstantBuffer()
{
    //创建常量缓冲区
    meshConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectNumber());

    CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    meshConstantBufferView.BuildConstantBuffer(desHandle, GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildMaterialConstantBuffer()
{
    //创建常量缓冲区
    materialConstantBufferView.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialObjectNumber());

    CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    materialConstantBufferView.BuildConstantBuffer(
        desHandle,
        GetDrawMaterialObjectNumber(),
        GetDrawMeshObjectNumber()
    );
}

void FGeometryMap::BuildLightConstantBuffer()
{
    //创建常量缓冲区
    lightConstantBufferView.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

    CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    lightConstantBufferView.BuildConstantBuffer(desHandle, GetDrawLightObjectNumber(), GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber());
}

void FGeometryMap::BuildViewportConstantBuffer()
{
    //创建常量缓冲区
    viewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 1);

    CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    //构建常量缓冲区
    viewportConstantBufferView.BuildConstantBuffer(desHandle, 1, GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber());
}

void FGeometryMap::BuildTextureConstantBuffer()
{
    renderingTextureResourcesUpdate->BuildTextureConstantBuffer(
        descriptorHeap.GetHeap(),
        GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber() + 1
    );
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
    return geometrys[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
    UINT count = 0;
    for (auto& tmp : geometrys)
    {
        for (auto& tmpSun : tmp.second.describeMeshRenderingData)
        {
            count += tmpSun.meshComp->GetMaterialNum();
        }
    }
    return count;
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
    return 1;
}

void FGeometryMap::DrawViewport(float deltaTime)
{
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

    desHandle.Offset(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber(), descriptorOffset);
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, desHandle);
}

void FGeometryMap::DrawLight(float deltaTime)
{
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

    desHandle.Offset(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber(), descriptorOffset);
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, desHandle);
}

void FGeometryMap::DrawMesh(float deltaTime)
{
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    for (auto& tmp : geometrys)
    {
        D3D12_VERTEX_BUFFER_VIEW vbv = tmp.second.GetVertexBufferView();
        D3D12_INDEX_BUFFER_VIEW ibv = tmp.second.GetIndexBufferView();
        for (int j = 0; j < tmp.second.describeMeshRenderingData.size(); ++j)
        {
            CD3DX12_GPU_DESCRIPTOR_HANDLE meshDesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
            CD3DX12_GPU_DESCRIPTOR_HANDLE materialDesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());

            FRenderingData& inRenderingData = tmp.second.describeMeshRenderingData[j];
            GetGraphicsCommandList()->IASetIndexBuffer(&ibv);
            //绑定渲染流水线是的输入槽,可以在输入装配阶段转入顶点数据
            GetGraphicsCommandList()->IASetVertexBuffers(
                0,//起始输入槽0-15
                1,
                &vbv
            );

            //定义要绘制哪种图元 点 线 面
            EMaterialDisplayStatusType displayStatusType = (*inRenderingData.meshComp->GetMaterials())[0]->GetMaterialDisplayStatusType();
            GetGraphicsCommandList()->IASetPrimitiveTopology(
                (D3D_PRIMITIVE_TOPOLOGY)displayStatusType
            );

            //模型起始地址偏移
            meshDesHandle.Offset(j, descriptorOffset);
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, meshDesHandle);

            //材质起始地址偏移
            materialDesHandle.Offset(GetDrawMeshObjectNumber() + j, descriptorOffset);
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, materialDesHandle);

            //真正绘制
            GetGraphicsCommandList()->DrawIndexedInstanced(
                inRenderingData.indexSize,//顶点索引数量
                1,//绘制数量
                inRenderingData.indexOffsetPosition,//顶点缓冲区第一个被绘制的索引
                inRenderingData.vertexOffsetPosition,//GPU从索引缓冲区读取的第一个索引位置
                0//在从顶点缓冲区中读取每个实例数据之前天道到每个索引的值
            );
        }
    }
}

void FGeometryMap::DrawTexture(float deltaTime)
{
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

    desHandle.Offset(
        GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber() + 1,
        descriptorOffset);
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(4, desHandle);
}

void FGeometryMap::LoadTexture()
{
    renderingTextureResourcesUpdate->LoadTextureResource(L"../JHSEngine/Asset/Texture/Texture1.dds");
}

bool FGeometry::bRenderingDataExistence(CMeshComponent* inKey)
{
    for (auto& tmp : describeMeshRenderingData)
    {
        if (tmp.meshComp == inKey)
            return true;
    }
    return false;
}

void FGeometry::BuildMesh(CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData)
{
    if (!bRenderingDataExistence(inMeshComponent))
    {
        describeMeshRenderingData.push_back(FRenderingData());
        FRenderingData& inRenderingData = describeMeshRenderingData[describeMeshRenderingData.size() - 1];

        //基础信息记录
        inRenderingData.meshComp = inMeshComponent;
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
