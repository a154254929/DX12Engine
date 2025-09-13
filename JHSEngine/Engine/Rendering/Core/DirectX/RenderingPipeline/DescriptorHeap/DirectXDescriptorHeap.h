#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FDirectXDescriptorHeap : public IDirectXDeviceInterface_Struct
{

	void Build(UINT inNumDescriptor);

	ID3D12DescriptorHeap* GetHeap() { return cbvHeap.Get();}
protected:
	ComPtr<ID3D12DescriptorHeap> cbvHeap;
};