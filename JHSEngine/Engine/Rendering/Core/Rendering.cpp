#include "Rendering.h"
#ifdef _WIN32
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#endif

vector<IRenderingInterface*> IRenderingInterface::renderingInterfaces;

IRenderingInterface::IRenderingInterface()
{
	renderingInterfaces.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	for (auto iter = renderingInterfaces.begin(); iter != renderingInterfaces.end(); ++iter)
	{
		if (*iter == this)
		{
			renderingInterfaces.erase(iter);
			break;
		}
	}
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float deltaTime)
{
	//重置命令列表
	ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));
}

void IRenderingInterface::Draw(float deltaTime)
{
}

void IRenderingInterface::PostDraw(float deltaTime)
{
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& outTmpBuffer, const void* inData, UINT64 inDataSize)
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

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		return inEngine->GetRenderingEngine()->d3dDevice;
	}
	return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		return inEngine->GetRenderingEngine()->graphicsCommandList;
	}
	return NULL;
}

ComPtr<ID3D12CommandAllocator> IRenderingInterface::GetCommandAllocator()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		return inEngine->GetRenderingEngine()->commandAllocator;
	}
	return NULL;
}

#ifdef _WIN32
CWindowsEngine* IRenderingInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(engine);
}
#else
CEngine* IRenderingInterface::GetEngine()
{
	return engine;
}
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
	elementSize = inElementSize;
	CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(inElementSize * inElementCount);
	ANALYSIS_HRESULT(inDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)
	));
	ANALYSIS_HRESULT(uploadBuffer->Map(0,nullptr,reinterpret_cast<void**>(&data)));
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
	return (inTypeSize + 255)& ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(elementSize);
}
