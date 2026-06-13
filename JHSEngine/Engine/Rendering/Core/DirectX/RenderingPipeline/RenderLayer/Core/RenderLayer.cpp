#include "RenderLayer.h"
#include "../RenderLayerManager.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"

FRenderLayer::FRenderLayer()
    : renderPriority(0)
{
}

void FRenderLayer::RegisterRenderLayer()
{
    FRenderLayerManager::renderLayers.push_back(this->shared_from_this());
}

void FRenderLayer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
    geometryMap = inGeometryMap;
    directXPipelineState = inDirectXPipelineState;
}

void FRenderLayer::PreDraw(float deltaTime)
{
}

void FRenderLayer::Draw(float deltaTime)
{
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    for (auto& renderingData : renderingDatas)
    {
        D3D12_VERTEX_BUFFER_VIEW vbv = geometryMap->geometrys[renderingData.geometryKey].GetVertexBufferView();
        D3D12_INDEX_BUFFER_VIEW ibv = geometryMap->geometrys[renderingData.geometryKey].GetIndexBufferView();
        
        CD3DX12_GPU_DESCRIPTOR_HANDLE meshDesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(geometryMap->descriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());

        GetGraphicsCommandList()->IASetIndexBuffer(&ibv);
        //绑定渲染流水线是的输入槽,可以在输入装配阶段转入顶点数据
        GetGraphicsCommandList()->IASetVertexBuffers(
            0,//起始输入槽0-15
            1,
            &vbv
        );

        //定义要绘制哪种图元 点 线 面
        EMaterialDisplayStatusType displayStatusType = (*renderingData.meshComp->GetMaterials())[0]->GetMaterialDisplayStatusType();
        GetGraphicsCommandList()->IASetPrimitiveTopology(
            (D3D_PRIMITIVE_TOPOLOGY)displayStatusType
        );

        //模型起始地址偏移
        meshDesHandle.Offset(renderingData.meshObjectIndex, descriptorOffset);
        GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, meshDesHandle);

        //真正绘制
        GetGraphicsCommandList()->DrawIndexedInstanced(
            renderingData.indexSize,//顶点索引数量
            1,//绘制数量
            renderingData.indexOffsetPosition,//顶点缓冲区第一个被绘制的索引
            renderingData.vertexOffsetPosition,//GPU从索引缓冲区读取的第一个索引位置
            0//在从顶点缓冲区中读取每个实例数据之前天道到每个索引的值
        );
    }
}

void FRenderLayer::PostDraw(float deltaTime)
{
}

void FRenderLayer::BuildPSO()
{
    BuildShader();
    
    //构建PSO
    directXPipelineState->BuildParam();
}

void FRenderLayer::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{

    int meshIndex = 0;
    for (auto& tmpRenderingData :renderingDatas)
    { 
        {
            XMFLOAT3& position = tmpRenderingData.meshComp->GetPosition();
            fvector_3d scale = tmpRenderingData.meshComp->GetScale();

            XMFLOAT3 rightVector = tmpRenderingData.meshComp->GetRightVector();
            XMFLOAT3 upVector = tmpRenderingData.meshComp->GetUpVector();
            XMFLOAT3 forwardVector = tmpRenderingData.meshComp->GetForwardVector();

            tmpRenderingData.worldMatrix = {
                rightVector.x * scale.x,    upVector.x,                forwardVector.x ,            0.f,
                rightVector.y,                upVector.y * scale.y,    forwardVector.y,            0.f,
                rightVector.z,                upVector.z ,            forwardVector.z * scale.z,    0.f,
                position.x,                    position.y,                position.z,                    1.f
            };
        }
        //更新模型位置
        XMMATRIX artixWorld = XMLoadFloat4x4(&tmpRenderingData.worldMatrix);
        XMMATRIX artixTextureTransfom = XMLoadFloat4x4(&tmpRenderingData.textureTransform);
        FObjectTransformation objectTransformation;
        XMStoreFloat4x4(&objectTransformation.world, XMMatrixTranspose(artixWorld));
        XMStoreFloat4x4(&objectTransformation.textureTransformation, XMMatrixTranspose(artixTextureTransfom));
        
        //收集材质Index
        if (auto &inMat = (*tmpRenderingData.meshComp->GetMaterials())[0])
        {
            objectTransformation.materialIndex = inMat->GetMaterialIndex();
        }
        
        geometryMap->meshConstantBufferView.Update(tmpRenderingData.meshObjectIndex, &objectTransformation);
        meshIndex++;
    }
}
