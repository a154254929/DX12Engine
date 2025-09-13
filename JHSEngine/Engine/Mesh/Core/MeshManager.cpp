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
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
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

void CMeshManager::BuildMesh(const FMeshRenderingData* inRenderingData)
{
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

    //构建根签名
    CD3DX12_ROOT_PARAMETER rootParam[2];

    //Object cbv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeObjCBV;
    descriptorRangeObjCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0
    );

    //ViewPort cbv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeViewportCBV;
    descriptorRangeViewportCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1
    );
    rootParam[0].InitAsDescriptorTable(1, &descriptorRangeObjCBV);
    rootParam[1].InitAsDescriptorTable(1, &descriptorRangeViewportCBV);

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        2,
        rootParam,
        0,
        nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    //创建
    ComPtr<ID3DBlob> serializeRootSignature;
    ComPtr<ID3DBlob> errorBlob;
    D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        serializeRootSignature.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    if (errorBlob)
    {
        Engine_Log_Error("%s", (char*)errorBlob->GetBufferPointer());
    }
    //创建
    GetD3dDevice()->CreateRootSignature(
        0,
        serializeRootSignature->GetBufferPointer(),
        serializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature)
    );

    //构建shader
    vertexShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "VertexShaderUnlit", "vs_5_0");
    pixelShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "PixelShaderUnlit", "ps_5_0");

    inputElementDesc = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };

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

    //PSO 流水线绑定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc;
    memset(&gpsDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    //绑定输入布局
    gpsDesc.InputLayout.pInputElementDescs = inputElementDesc.data();
    gpsDesc.InputLayout.NumElements = (UINT)inputElementDesc.size();

    //绑定根签名
    gpsDesc.pRootSignature = rootSignature.Get();

    //绑定顶点着色器
    gpsDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(vertexShader.GetBufferPointer());
    gpsDesc.VS.BytecodeLength = vertexShader.GetBufferSize();

    //绑定像素着色器
    gpsDesc.PS.pShaderBytecode = pixelShader.GetBufferPointer();
    gpsDesc.PS.BytecodeLength = pixelShader.GetBufferSize();

    //配置光栅化状态
    gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//线框方式渲染

    gpsDesc.SampleMask = UINT_MAX;

    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    gpsDesc.NumRenderTargets = 1;

    gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    gpsDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    gpsDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();
    gpsDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();

    gpsDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();
    gpsDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilBufferFormat();

    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pipelineStatePSO)))
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

    renderingPipeline->BuildMesh(myMesh, &meshRenderingData);

    myMesh->Init();

    return myMesh;
}