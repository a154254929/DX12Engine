#include "DirectXPipelineState.h"
#include "../../../../Engine/DirectX/Core/DirectXRenderingEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
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

    gpsDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();
    gpsDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilBufferFormat();

    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pipelineStatePSO)))
}
