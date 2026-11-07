#include "NormalBufferRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FNormalBufferRenderLayer::FNormalBufferRenderLayer()
{
    renderPriority = 5000;
}

void FNormalBufferRenderLayer::Draw(float deltaTime)
{
    ResetPSO();
    Super::Draw(deltaTime);
}

void FNormalBufferRenderLayer::BuildShader()
{
    std::vector<ShaderType::FShaderMacro> shaderMacros;
    BuildShaderMacro(shaderMacros);
    
    std::vector<D3D_SHADER_MACRO> d3DShaderMacro;
    ShaderType::ToD3DShaderMacro(shaderMacros, d3DShaderMacro);
    
    std:: wstring shaderPath = BuildShadersPaths(L"Unlit");
    vertexShader.BuildShaders(
        shaderPath,
        "VertexShaderUnlit",
        "vs_5_1",
        d3DShaderMacro.data()
    );
    pixelShader.BuildShaders(
        shaderPath,
        "PixelShaderUnlit",
        "ps_5_1",
        d3DShaderMacro.data()
    );
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

void FNormalBufferRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC& psoDesc = directXPipelineState->GetGPSDesc();
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    
    directXPipelineState->Build(EPipelineState::NormalBuffer);
}

void FNormalBufferRenderLayer::ResetPSO()
{
    directXPipelineState->Build(EPipelineState::NormalBuffer);
}
