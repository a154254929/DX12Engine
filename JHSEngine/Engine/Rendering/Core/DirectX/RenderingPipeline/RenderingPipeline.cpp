#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(GMesh* inMesh, const FMeshRenderingData& inMeshData)
{
	geometryMap.BuildMesh(inMesh, inMeshData);
}

void FRenderingPipeline::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
	geometryMap.UpdateCalculations(deltaTime, viewportInfo);
}


void FRenderingPipeline::BuildPipeline()
{
	//初始化GPS描述
	directXPipelineState.ResetGPSDesc();

	//构建根签名
	rootSignature.BuildRootSignature();
	directXPipelineState.BindRootSignature(rootSignature.GetRootSignature());

	//构建shader
	vertexShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "VertexShaderUnlit", "vs_5_0");
	pixelShader.BuildShaders(L"../JHSEngine/Shader/Unlit.hlsl", "PixelShaderUnlit", "ps_5_0");
	directXPipelineState.BindShader(vertexShader, pixelShader);

	//输入布局
	inputElementDesc = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	directXPipelineState.BindInputLayout(inputElementDesc.data(), inputElementDesc.size());

	//构建模型
	geometryMap.Build();

	//构建常量描述堆
	geometryMap.BuildDescriptorHeap();

	//构建模型常量缓冲区
	geometryMap.BuildMeshConstantBuffer();

	//构建材质常量缓冲区
	geometryMap.BuildMaterialConstantBuffer();

	//构建光源常量缓冲区
	geometryMap.BuildLightConstantBuffer();

	//构建视口常量缓冲区
	geometryMap.BuildViewportConstantBuffer();

	//构建管线
	directXPipelineState.Build();
}

void FRenderingPipeline::PreDraw(float deltaTime)
{
	directXPipelineState.PreDraw(deltaTime);
}

void FRenderingPipeline::Draw(float deltaTime)
{
	rootSignature.PreDraw(deltaTime);
	geometryMap.PreDraw(deltaTime);

	geometryMap.Draw(deltaTime);
	directXPipelineState.Draw(deltaTime);
}

void FRenderingPipeline::PostDraw(float deltaTime)
{
	geometryMap.PostDraw(deltaTime);
	directXPipelineState.PostDraw(deltaTime);
}
