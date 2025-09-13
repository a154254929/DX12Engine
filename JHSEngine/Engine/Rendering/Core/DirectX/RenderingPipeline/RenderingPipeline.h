#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"

class CMesh;
class FRenderingPipeline: public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* inMesh, const FMeshRenderingData& inMeshData);

	void BuildPipeline();
protected:
	FGeometryMap geometryMap;

	FShader vertexShader;
	FShader pixelShader;

	vector<D3D12_INPUT_ELEMENT_DESC> inputElementDesc;

	FDirectXPipelineState directXPipelineState;
	FDirectXRootSignature rootSignature;
};