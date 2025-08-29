#include "ConstructBuffer.h"

namespace ConstructBuffer
{

	ComPtr<ID3D12Resource> FConstructBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& outTmpBuffer, const void* inData, UINT64 inDataSize)
	{
		ComPtr<ID3D12Resource> buffer;
		CD3DX12_RESOURCE_DESC bufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(inDataSize);
		CD3DX12_HEAP_PROPERTIES bufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
			&bufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&bufferResourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			NULL,
			IID_PPV_ARGS(buffer.GetAddressOf())
		));

		CD3DX12_HEAP_PROPERTIES updateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
			&updateBufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&bufferResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			NULL,
			IID_PPV_ARGS(outTmpBuffer.GetAddressOf())
		));

		D3D12_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pData = inData;
		subresourceData.RowPitch = inDataSize;
		subresourceData.SlicePitch = subresourceData.RowPitch;

		//标记资源为复制目标
		CD3DX12_RESOURCE_BARRIER copyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		);

		GetGraphicsCommandList()->ResourceBarrier(1, &copyDestBarrier);

		UpdateSubresources<1>(
			GetGraphicsCommandList().Get(),
			buffer.Get(),
			outTmpBuffer.Get(),
			0,//0 -> D3D12_REQ_SUBRESOURCES
			0,//0 -> D3D12_REQ_SUBRESOURCES
			1,
			&subresourceData
		);

		CD3DX12_RESOURCE_BARRIER readDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ
		);

		return buffer;
	}
}