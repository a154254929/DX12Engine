#include "BackGroundRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FBackGroundRenderLayer::FBackGroundRenderLayer()
{
    renderPriority = 1000;
}

void FBackGroundRenderLayer::Draw(float deltaTime)
{
    directXPipelineState->ResetPSO(BackGround);
    
    Super::Draw(deltaTime);
}

void FBackGroundRenderLayer::BuildShader()
{
    std::vector<ShaderType::FShaderMacro> shaderMacros;
    BuildShaderMacro(shaderMacros);
    
    std::vector<D3D_SHADER_MACRO> d3DShaderMacro;
    ShaderType::ToD3DShaderMacro(shaderMacros, d3DShaderMacro);
    
    vertexShader.BuildShaders(L"../JHSEngine/Shader/Sky.hlsl", "VertexShaderSky", "vs_5_1", d3DShaderMacro.data());
    pixelShader.BuildShaders(L"../JHSEngine/Shader/Sky.hlsl", "PixelShaderSky", "ps_5_1", d3DShaderMacro.data());
    directXPipelineState->BindShader(vertexShader, pixelShader);

    //输入布局
    inputElementDesc = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };
    directXPipelineState->BindInputLayout(inputElementDesc.data(), inputElementDesc.size());
}

void FBackGroundRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    
    CD3DX12_RASTERIZER_DESC rasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    directXPipelineState->SetRasterizerState(rasterizerDesc);
    CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    directXPipelineState->SetDepthStencilState(depthStencilDesc);
    
    //构建管线
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::BackGround);
}
