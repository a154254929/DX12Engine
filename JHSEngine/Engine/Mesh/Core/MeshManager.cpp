#include "MeshManager.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"

CMeshManager::CMeshManager()
    : vertexSizeInBytes(0)
    , vertexStrideInBytes(0)
    , indexSizeInBytes(0)
    , indexFormat(DXGI_FORMAT_R16_UINT)
    , indexSize(0)
    , worldMatrix(FObjectTransformation::IdentityMatrix4x4())
    , viewMatrix(FObjectTransformation::IdentityMatrix4x4())
    , projectMatrix(FObjectTransformation::IdentityMatrix4x4())
{

}

void CMeshManager::Init()
{
    float aspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScreenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    XMMATRIX project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI, //以弧度为单位的自上而下的视场角
        aspectRatio,//宽高比
        1.0f,//近裁剪面
        1000.f//原剪裁面
    );

    XMStoreFloat4x4(&projectMatrix, project);
}

void CMeshManager::BuildMesh(const FMeshRenderingData* inRenderingData)
{
    //构建栈
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    heapDesc.NumDescriptors = 1;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    heapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&cbvHeap));

    //常量缓冲区的构建
    objectConstants = make_shared<FRenderingResourcesUpdate>();
    objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS obAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = obAddr;
    cbvDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();

    GetD3dDevice()->CreateConstantBufferView(
        &cbvDesc,
        cbvHeap->GetCPUDescriptorHandleForHeapStart()
    );

    //构建根签名
    CD3DX12_ROOT_PARAMETER rootParam[1];

    //cbv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeCBV;
    descriptorRangeCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0
    );
    rootParam[0].InitAsDescriptorTable(1, &descriptorRangeCBV);

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        1,
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

    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, cbvHeap->GetGPUDescriptorHandleForHeapStart());

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
    XMINT3 cameraPos = XMINT3(5.0f, 5.0f, 5.0f);

    XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
    XMVECTOR viewTarget = XMVectorZero();
    XMVECTOR viewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

    XMMATRIX viewLookAt = XMMatrixLookAtLH(pos, viewTarget, viewUp);
    XMStoreFloat4x4(&viewMatrix, viewLookAt);

    XMMATRIX artixWorld = XMLoadFloat4x4(&worldMatrix);
    XMMATRIX artixProject = XMLoadFloat4x4(&projectMatrix);
    XMMATRIX wvp = artixWorld * viewLookAt * artixProject;

    FObjectTransformation objectTransformation;
    XMStoreFloat4x4(&objectTransformation.world, XMMatrixTranspose(wvp));
    objectConstants->Update(0, &objectTransformation);
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

template<class T, typename ...ParamTypes>
T* CMeshManager::CreateMesh(ParamTypes && ...params)
{
    T* MyMesh = new T();

    FMeshRenderingData meshRenderingData;
    MyMesh->CreateMesh(meshRenderingData, forward<ParamTypes>(params)...);

    MyMesh->BeginInit();

    BuildMesh(&meshRenderingData);

    MyMesh->Init();

    return MyMesh;
}