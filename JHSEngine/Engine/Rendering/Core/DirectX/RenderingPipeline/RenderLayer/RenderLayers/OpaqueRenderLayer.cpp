#include "OpaqueRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
    renderPriority = 2000;
}

void FOpaqueRenderLayer::Draw(float deltaTime)
{
    directXPipelineState->ResetPSO();
    Super::Draw(deltaTime);
}

void FOpaqueRenderLayer::BuildShader()
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

void FOpaqueRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::Opaque);
    
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::GrayModel);
    //构建管线
    directXPipelineState->SetFillMode(true);
    directXPipelineState->Build(EPipelineState::WireFrame);
}
