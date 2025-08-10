#pragma once
#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#include "../../Core/Engine.h"
#endif
class IRenderingInterface
{
	friend class CWindowsEngine;
public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	bool operator==(const IRenderingInterface& inOther)
	{
		return guid_equal(&guid, &inOther.guid);
	}

	simple_c_guid GetGuid() { return guid; }

protected:
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& outTmpBuffer, const void* inData, UINT64 inDataSize);
protected:
	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
#ifdef _WIN32
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();
#endif
private:
	static vector<IRenderingInterface*> renderingInterfaces;

	simple_c_guid guid;
};

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