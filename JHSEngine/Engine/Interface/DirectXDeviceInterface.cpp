#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#ifdef _WIN32
#include "../Platform/Windows/WindowsEngine.h"
#else
#include "../Core/Engine.h"
#endif

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->fence;
		}
	}
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->d3dDevice;
		}
	}
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->commandQueue;
		}
	}
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->commandAllocator;
		}
	}
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->graphicsCommandList;
		}
	}
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->currentFenceIndex;
		}
	}
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
{
#ifdef _WIN32
	if (CWindowsEngine* inEngine = GetEngine())
#else
	if (CEngine* inEngine = GetEngine())
#endif
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->mainWindowsHandle;
		}
	}
	return HWND();
}

#ifdef _WIN32
CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(engine);
}
#else
CEngine* IDirectXDeviceInterface::GetEngine()
{
	return engine;
}
#endif

ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence()
{
	return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice()
{
	return Interface.GetD3dDevice();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue()
{
	return Interface.GetCommandQueue();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator()
{
	return Interface.GetCommandAllocator();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList()
{
	return Interface.GetGraphicsCommandList();
}

#ifdef _WIN32
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
	return Interface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
	return Interface.GetEngine();
}
#endif