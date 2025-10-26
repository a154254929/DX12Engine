#include "DirectXPipelineState.h"
#include "../../../../Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{
    //线框
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>()));
    //Shader
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>()));
}

void FDirectXPipelineState::PreDraw(float deltaTime)
{
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[pipelineState].Get());
}

void FDirectXPipelineState::Draw(float deltaTime)
{
    //捕获键盘按键
    CaptureKeyboardKeys();
}

void FDirectXPipelineState::PostDraw(float deltaTime)
{
}

void FDirectXPipelineState::ResetGPSDesc()
{
	memset(&gpsDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* inInputElementDescs, UINT inSize)
{
    //绑定输入布局
    gpsDesc.InputLayout.pInputElementDescs = inInputElementDescs;
    gpsDesc.InputLayout.NumElements = inSize;
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* inRootSignature)
{
    //绑定根签名
    gpsDesc.pRootSignature = inRootSignature;
}

void FDirectXPipelineState::BindShader(const FShader& inVertextShader, const FShader& inPixelShader)
{
    //绑定顶点着色器
    gpsDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(inVertextShader.GetBufferPointer());
    gpsDesc.VS.BytecodeLength = inVertextShader.GetBufferSize();

    //绑定像素着色器
    gpsDesc.PS.pShaderBytecode = inPixelShader.GetBufferPointer();
    gpsDesc.PS.BytecodeLength = inPixelShader.GetBufferSize();
}

void FDirectXPipelineState::Build()
{

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

    //RTV和DSV格式
    gpsDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();
    gpsDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilBufferFormat();
    
    //线框模式注册
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&PSO[EPipelineState::WireFrame])))

    //实体模式注册
    gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//实体方式渲染
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&PSO[EPipelineState::GrayModel])))
}

void FDirectXPipelineState::CaptureKeyboardKeys()
{
    if (GetAsyncKeyState('4') & 0x8000)
    {
        pipelineState = EPipelineState::GrayModel;
    }
    else if (GetAsyncKeyState('5') & 0x8000)
    {
        pipelineState = EPipelineState::WireFrame;
    }
}
