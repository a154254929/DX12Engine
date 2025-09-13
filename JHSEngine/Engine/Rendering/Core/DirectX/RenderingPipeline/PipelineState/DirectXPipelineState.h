#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Shader/Core/Shader.h"

struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	void ResetGPSDesc();

	//绑定输入布局
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* inInputElementDescs, UINT inSize);

	//绑定根签名
	void BindRootSignature(ID3D12RootSignature* inRootSignature);

	//绑定顶点着色器和像素着色器
	void BindShader(const FShader& inVertextShader, const FShader& inPixelShader);

	void Build();
private:
	ComPtr<ID3D12PipelineState> pipelineStatePSO;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc;
};