#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
	FDirectXRootSignature();

	void BuildRootSignature();

	void PreDraw(float deltaTime);
	void Draw(float deltaTime);
	void PostDraw(float deltaTime);

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
public:
	ComPtr<ID3D12RootSignature> rootSignature;

};