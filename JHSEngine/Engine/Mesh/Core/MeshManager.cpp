#include "MeshManager.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "../AesmaMesh.h"
#include "../DonutMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Core/Viewport/ViewportTransformation.h"

CMeshManager::CMeshManager()
    : vertexSizeInBytes(0)
    , vertexStrideInBytes(0)
    , indexSizeInBytes(0)
    , indexFormat(DXGI_FORMAT_R16_UINT)
    , indexSize(0)
    , worldMatrix(EngineMath::IdentityMatrix4x4())
{

}

void CMeshManager::Init()
{
}

void CMeshManager::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
   //XMINT3 cameraPos = XMINT3(viewportInfo.viewMatrix.m[3][0], viewportInfo.viewMatrix.m[3][1], viewportInfo.viewMatrix.m[3][2]);
   //XMINT3 cameraPos = XMINT3(5.0f, 5.0f, 5.0f);

    //XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
    //XMVECTOR viewTarget = XMVectorZero();
    //XMVECTOR viewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

    //XMMATRIX viewLookAt = XMMatrixLookAtLH(pos, viewTarget, viewUp);
    //XMStoreFloat4x4(const_cast<XMFLOAT4X4*>(&viewportInfo.viewMatrix), viewLookAt);
    //XMMATRIX viewLookAt = XMLoadFloat4x4(&viewportInfo.viewMatrix);

    //XMMATRIX artixWorld = XMLoadFloat4x4(&worldMatrix);
    //XMMATRIX artixProject = XMLoadFloat4x4(&viewportInfo.projectMatrix);
    //XMMATRIX wvp = artixWorld * viewLookAt * artixProject;

    XMMATRIX wrold = XMLoadFloat4x4(&worldMatrix);

    FObjectTransformation objectTransformation;
    XMStoreFloat4x4(&objectTransformation.world, XMMatrixTranspose(wrold));
    objectConstants->Update(0, &objectTransformation);

    //更新视口
    XMMATRIX projectMatrix = XMLoadFloat4x4(&viewportInfo.projectMatrix);
    XMMATRIX viewMatrix = XMLoadFloat4x4(&viewportInfo.viewMatrix);
    XMMATRIX viewProjMatrix = XMMatrixMultiply(viewMatrix, projectMatrix);

    FViewportTransformation viewportTransformation;
    XMStoreFloat4x4(&viewportTransformation.viewProjectionMatrix, XMMatrixTranspose(viewProjMatrix));
    viewportConstants->Update(0, &viewportTransformation);
}

void CMeshManager::BuildMesh()
{
    renderingPipeline.BuildPipeline();
    return;

    //构建栈
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    heapDesc.NumDescriptors = 2;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    heapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&cbvHeap));

    //构建常量缓冲区
    {
        UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cbvHeap->GetCPUDescriptorHandleForHeapStart());

        //构建对象常量缓冲区
        objectConstants = make_shared<FRenderingResourcesUpdate>();
        objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
        D3D12_GPU_VIRTUAL_ADDRESS objAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

        D3D12_CONSTANT_BUFFER_VIEW_DESC objCBVDesc;
        objCBVDesc.BufferLocation = objAddr;
        objCBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();

        desHandle.Offset(0, descriptorOffset);
        GetD3dDevice()->CreateConstantBufferView(
            &objCBVDesc,
            desHandle
        );

        //构建视口常量缓冲区
        viewportConstants = make_shared<FRenderingResourcesUpdate>();
        viewportConstants->Init(GetD3dDevice().Get(), sizeof(FViewportTransformation), 1);
        D3D12_GPU_VIRTUAL_ADDRESS viewportAddr = viewportConstants.get()->GetBuffer()->GetGPUVirtualAddress();

        D3D12_CONSTANT_BUFFER_VIEW_DESC viewportCBVDesc;
        viewportCBVDesc.BufferLocation = viewportAddr;
        viewportCBVDesc.SizeInBytes = viewportConstants->GetConstantBufferByteSize();

        desHandle.Offset(1, descriptorOffset);
        GetD3dDevice()->CreateConstantBufferView(
            &viewportCBVDesc,
            desHandle
        );
    }

    ////////////////////////////
    //构建模型
    vertexStrideInBytes = sizeof(FVertex);
    indexSize = inRenderingData->indexData.size();

    //获取模型数据大小
    vertexSizeInBytes = inRenderingData->vertexData.size() * vertexStrideInBytes;
    indexSizeInBytes = indexSize * sizeof(uint16_t);

    ANALYSIS_HRESULT(D3DCreateBlob(vertexSizeInBytes, &cpuVertexBufferPtr));
    memcpy(
        cpuVertexBufferPtr->GetBufferPointer(),
        inRenderingData->vertexData.data(),
        vertexSizeInBytes
    );

    ANALYSIS_HRESULT(D3DCreateBlob(indexSizeInBytes, &cpuIndexBufferPtr));
    memcpy(
        cpuIndexBufferPtr->GetBufferPointer(),
        inRenderingData->indexData.data(),
        indexSizeInBytes
    );

    ConstructBuffer::FConstructBuffer constructBuffer;
    gpuVertexBufferPtr = constructBuffer.ConstructDefaultBuffer(
        vertexBufferTmpPtr,
        inRenderingData->vertexData.data(),
        vertexSizeInBytes
    );
    gpuIndexBufferPtr = constructBuffer.ConstructDefaultBuffer(
        indexBufferTmpPtr,
        inRenderingData->indexData.data(),
        indexSizeInBytes
    );
}

void CMeshManager::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), pipelineStatePSO.Get());
}

void CMeshManager::Draw(float DeltaTime)
{
    ID3D12DescriptorHeap* descriptorHeap[] = { cbvHeap.Get() };
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(descriptorHeap), descriptorHeap);

    GetGraphicsCommandList()->SetGraphicsRootSignature(rootSignature.Get());

    D3D12_VERTEX_BUFFER_VIEW vbv = GetVertexBufferView();
    //绑定渲染流水线是的输入槽,可以在输入装配阶段转入顶点数据
    GetGraphicsCommandList()->IASetVertexBuffers(
        0,//起始输入槽0-15
        1,
        &vbv
    );

    D3D12_INDEX_BUFFER_VIEW ibv = GetIndexBufferView();
    GetGraphicsCommandList()->IASetIndexBuffer(&ibv);

    //定义要绘制哪种图元 点 线 面
    GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart());

    desHandle.Offset(0, descriptorOffset);
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, desHandle);
    desHandle.Offset(1, descriptorOffset);
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, desHandle);

    //真正绘制
    GetGraphicsCommandList()->DrawIndexedInstanced(
        indexSize,//顶点索引数量
        1,//绘制数量
        0,//顶点缓冲区第一个被绘制的索引
        0,//GPU从所以缓冲区读取的第一个索引位置
        0//在从顶点缓冲区中读取每个实例数据之前天道到每个索引的值
    );
}

void CMeshManager::PostDraw(float DeltaTime)
{
}

D3D12_VERTEX_BUFFER_VIEW CMeshManager::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW vbv;
    vbv.BufferLocation = gpuVertexBufferPtr->GetGPUVirtualAddress();
    vbv.SizeInBytes = vertexSizeInBytes;
    vbv.StrideInBytes = vertexStrideInBytes;
    return vbv;
}

D3D12_INDEX_BUFFER_VIEW CMeshManager::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW ibv;
    ibv.BufferLocation = gpuIndexBufferPtr->GetGPUVirtualAddress();
    ibv.SizeInBytes = indexSizeInBytes;
    ibv.Format = indexFormat;
    return ibv;
}

CMesh* CMeshManager::CreateSphereMesh(float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CSphereMesh>(inRadius, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreateMesh(string& inPath)
{
    return CreateMesh<CCustomMesh>(inPath);
}

CMesh* CMeshManager::CreateBoxMesh(float inHeight, float inWidth, float inDepth)
{
    return CreateMesh<CBoxMesh>(inDepth, inDepth, inDepth);
}

CMesh* CMeshManager::CreateConeMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CConeMesh>(inRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreateCylinderMesh(float inTopRadius, float inBottomRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CCylinderMesh>(inTopRadius, inBottomRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreatePlaneMesh(float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide)
{
    return CreateMesh<CPlaneMesh>(inHeight, inWidth, inHeightSubdivide, inWidthSubdivide);
}

CMesh* CMeshManager::CreateAesmaMesh(float inOuterRadius, float inInnerRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision)
{
    return CreateMesh<CAesmaMesh>(inOuterRadius, inInnerRadius, inHeight, inAxialSubdivision, inHeightSubdivision);
}

CMesh* CMeshManager::CreateDonutMesh(float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision)
{
    return CreateMesh<CDonutMesh>(inDonutRadius, inThicknessRadius, inDonutAxialSubdivision, inThicknessAxialSubdivision);
}

template<class T, typename ...ParamTypes>
T* CMeshManager::CreateMesh(ParamTypes && ...params)
{
    T* myMesh = new T();

    FMeshRenderingData meshRenderingData;
    myMesh->CreateMesh(meshRenderingData, forward<ParamTypes>(params)...);

    myMesh->BeginInit();

    renderingPipeline->BuildMesh(myMesh, meshRenderingData);

    myMesh->Init();

    return myMesh;
}