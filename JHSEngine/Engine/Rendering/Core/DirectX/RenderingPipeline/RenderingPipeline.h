#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"

class GMesh;
class FRenderingPipeline: public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(GMesh* inMesh, const FMeshRenderingData& inMeshData);

	virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);

	void BuildPipeline();

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);
protected:
	FGeometryMap geometryMap;

	FShader vertexShader;
	FShader pixelShader;

	vector<D3D12_INPUT_ELEMENT_DESC> inputElementDesc;

	FDirectXPipelineState directXPipelineState;
	FDirectXRootSignature rootSignature;
};