#include "RenderingResourcesUpdate.h"
#ifdef _WIN32
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#endif


FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{
}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (uploadBuffer != nullptr)
	{
		uploadBuffer->Unmap(0, NULL);
		uploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* inDevice, UINT inElementSize, UINT inElementCount)
{
	assert(inDevice);
	elementSize = GetConstantBufferByteSize(inElementSize);
	CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(elementSize * inElementCount);
	ANALYSIS_HRESULT(inDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)
	));
	ANALYSIS_HRESULT(uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data)));
}

void FRenderingResourcesUpdate::Update(int index, const void* inData)
{
	memcpy(&data[index * elementSize], inData, elementSize);
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT inTypeSize)
{
	/*if (!(inTypeSize % 256))
	{
		float newFloat = (float)inTypeSize / 256.f;
		int num = newFloat + 1;
		return num * 256;
	}
	return inTypeSize;
	*/
	return (inTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(elementSize);
}