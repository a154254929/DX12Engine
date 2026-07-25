#include "OpaqueShadowRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueShadowRenderLayer::FOpaqueShadowRenderLayer()
{
    renderPriority = 1000;
}

void FOpaqueShadowRenderLayer::Draw(float deltaTime)
{
    ResetPSO();
    Super::Draw(deltaTime);
}

void FOpaqueShadowRenderLayer::BuildShader()
{
    std::vector<ShaderType::FShaderMacro> shaderMacros;
    BuildShaderMacro(shaderMacros);
    
    std::vector<D3D_SHADER_MACRO> d3DShaderMacro;
    ShaderType::ToD3DShaderMacro(shaderMacros, d3DShaderMacro);
    
    vertexShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "VertexShaderUnlit", "vs_5_1", d3DShaderMacro.data());
    pixelShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "PixelShaderUnlit", "ps_5_1", d3DShaderMacro.data());
    directXPipelineState->BindShader(vertexShader, pixelShader);

    //输入布局
    inputElementDesc = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };
    directXPipelineState->BindInputLayout(inputElementDesc.data(), inputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = directXPipelineState->GetGPSDesc();
    gpsDesc.RasterizerState.DepthBias = 100000;
    gpsDesc.RasterizerState.DepthBiasClamp = 0.0f;
    gpsDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;
    
    //关闭Render Target
    gpsDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
    gpsDesc.NumRenderTargets = 0;
    
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::Opaque);
}

void FOpaqueShadowRenderLayer::ResetPSO()
{
    directXPipelineState->ResetPSO(Shadow);
}
