#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../StaticSampler/StaticSampler.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
    FDirectXRootSignature();

    void BuildRootSignature(UINT inTexture2DNum = 1, UINT inTextureCubemapNum = 1);

    void PreDraw(float deltaTime);
    void Draw(float deltaTime);
    void PostDraw(float deltaTime);

    ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
public:
    ComPtr<ID3D12RootSignature> rootSignature;
    FStaticSamplerObject staticSamplerObject;
};