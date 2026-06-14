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
    char textureNumBuff[10] = {0};
    //构建shader
    D3D_SHADER_MACRO shaderMacro[] = {
        "Texture2DMap_Count", _itoa(geometryMap->GetDrawTexture2DResourcesNumber(), textureNumBuff, 10),
        NULL, NULL
    };
    vertexShader.BuildShaders(L"../JHSEngine/Shader/Sky.hlsl", "VertexShaderUnlit", "vs_5_1", shaderMacro);
    pixelShader.BuildShaders(L"../JHSEngine/Shader/Sky.hlsl", "PixelShaderUnlit", "ps_5_1", shaderMacro);
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
