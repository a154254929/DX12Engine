#include "TransparentRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
    renderPriority = 3000;
}

void FTransparentRenderLayer::Draw(float deltaTime)
{
    directXPipelineState->ResetPSO(Transparent);
    
    Super::Draw(deltaTime);
}

void FTransparentRenderLayer::BuildShader()
{
    char textureNumBuff[10] = {0};
    //构建shader
    D3D_SHADER_MACRO shaderMacro[] = {
        "Texture2DMap_Count", _itoa(geometryMap->GetDrawTexture2DResourcesNumber(), textureNumBuff, 10),
        NULL, NULL
    };
    vertexShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "VertexShaderUnlit", "vs_5_1", shaderMacro);
    pixelShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "PixelShaderUnlit", "ps_5_1", shaderMacro);
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

void FTransparentRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {
        true,
        false,
        D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    
    directXPipelineState->SetRenderTarget(0, blendDesc);
    
    //构建管线
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::Transparent);
}
