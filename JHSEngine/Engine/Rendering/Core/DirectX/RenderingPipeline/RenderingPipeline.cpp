#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData)
{
	geometryMap.BuildMesh(inMesh, inMeshData);
}


void FRenderingPipeline::BuildPipeline()
{
	//初始化GPS描述
	directXPipelineState.ResetGPSDesc();

	//构建根签名
	rootSignature.BuildRootSignature();
	directXPipelineState.BindRootSignature(rootSignature.GetRootSignature());


	//构建管线
	directXPipelineState.Build();
}
