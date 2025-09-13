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
	directXPipelineState.ResetGPSDesc();

	directXPipelineState.BindRootSignature();


	//构建管线
	directXPipelineState.Build();
}
