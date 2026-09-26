#include "OperationHandleRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FOperationHandleRenderLayer::FOperationHandleRenderLayer()
{
    renderPriority = 5000;
}

void FOperationHandleRenderLayer::Draw(float deltaTime)
{
    ResetPSO();
    Super::Draw(deltaTime);
}

void FOperationHandleRenderLayer::BuildShader()
{
    std::vector<ShaderType::FShaderMacro> shaderMacros;
    BuildShaderMacro(shaderMacros);
    
    std::vector<D3D_SHADER_MACRO> d3DShaderMacro;
    ShaderType::ToD3DShaderMacro(shaderMacros, d3DShaderMacro);
    
    std:: wstring shaderPath = BuildShadersPaths(L"OperationHandle");
    vertexShader.BuildShaders(
        shaderPath,
        "VertexShaderOperationHandle",
        "vs_5_1",
        d3DShaderMacro.data()
    );
    pixelShader.BuildShaders(
        shaderPath,
        "PixelShaderOperationHandle",
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

void FOperationHandleRenderLayer::BuildPSO()
{
    Super::BuildPSO();

    directXPipelineState->Build(EPipelineState::OperationHandle);
}

void FOperationHandleRenderLayer::ResetPSO()
{
    directXPipelineState->ResetPSO(EPipelineState::OperationHandle);
}
