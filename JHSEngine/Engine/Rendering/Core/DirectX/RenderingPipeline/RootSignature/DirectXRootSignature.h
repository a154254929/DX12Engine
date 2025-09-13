#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
	FDirectXRootSignature();

	void BuildRootSignature();

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
public:
	ComPtr<ID3D12RootSignature> rootSignature;

};