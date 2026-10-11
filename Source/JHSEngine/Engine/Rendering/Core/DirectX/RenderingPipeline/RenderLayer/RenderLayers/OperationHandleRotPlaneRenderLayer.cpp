#include "OperationHandleRotPlaneRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FOperationHandleRotPlaneRenderLayer::FOperationHandleRotPlaneRenderLayer()
{
    renderPriority = 5000;
}

void FOperationHandleRotPlaneRenderLayer::Draw(float deltaTime)
{
    ResetPSO();
    Super::Draw(deltaTime);
}

void FOperationHandleRotPlaneRenderLayer::BuildShader()
{
    std::vector<ShaderType::FShaderMacro> shaderMacros;
    BuildShaderMacro(shaderMacros);
    
    std::vector<D3D_SHADER_MACRO> d3DShaderMacro;
    ShaderType::ToD3DShaderMacro(shaderMacros, d3DShaderMacro);
    
    std:: wstring shaderPath = BuildShadersPaths(L"OperationRotPlaneHandle");
    vertexShader.BuildShaders(
        shaderPath,
        "VertexShaderOperationRotPlaneHandle",
        "vs_5_1",
        d3DShaderMacro.data()
    );
    pixelShader.BuildShaders(
        shaderPath,
        "PixelShaderOperationRotPlaneHandle",
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

void FOperationHandleRotPlaneRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    
    CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    depthStencilDesc.StencilEnable = FALSE;
    
    directXPipelineState->SetDepthStencilState(depthStencilDesc);
    
    CD3DX12_RASTERIZER_DESC rasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    directXPipelineState->SetRasterizerState(rasterizerDesc);
    
    D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {
        true,
        false,
        D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    directXPipelineState->SetRenderTarget(0, blendDesc);

    directXPipelineState->Build(EPipelineState::OperationHandleRotPlane);
}

void FOperationHandleRotPlaneRenderLayer::ResetPSO()
{
    directXPipelineState->ResetPSO(EPipelineState::OperationHandleRotPlane);
}
