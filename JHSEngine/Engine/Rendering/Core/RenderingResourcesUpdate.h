#pragma once
class FRenderingResourcesUpdate : public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* inDevice, UINT inElemetSize, UINT inElemetCount);

	void Update(int index, const void* inData);

	UINT GetConstantBufferByteSize(UINT inTypeSize);
	UINT GetConstantBufferByteSize();

	ID3D12Resource* GetBuffer() { return uploadBuffer.Get(); }
private:
	ComPtr<ID3D12Resource> uploadBuffer;
	UINT elementSize;
	BYTE* data;
};
