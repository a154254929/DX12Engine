#pragma once
#include "../../../Core//Engine.h"
#include "../../../Core/Engine.h"
#include "../../../Interface/DirectXDeviceInterface.h"

namespace ConstructBuffer
{
	struct FConstructBuffer:IDirectXDeviceInterface_Struct
	{
		ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& outTmpBuffer, const void* inData, UINT64 inDataSize);
	};
}