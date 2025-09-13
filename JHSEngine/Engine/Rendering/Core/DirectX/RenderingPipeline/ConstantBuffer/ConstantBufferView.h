#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

struct FConstantBufferView : public IDirectXDeviceInterface_Struct
{
	void CreateConstant(UINT objectSize, UINT objectCount);

	void BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE inHandle, UINT inConstantBufferNum, UINT inHandleOffset = 0);

	void Update(int index, const void* inData);
protected:
	shared_ptr<FRenderingResourcesUpdate> constants;
};