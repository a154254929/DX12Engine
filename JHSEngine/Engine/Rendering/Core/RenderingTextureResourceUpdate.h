#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterface.h"

struct FRenderingTexture
{
    ComPtr<ID3D12Resource> uploadBuffer;
    ComPtr<ID3D12Resource> data;
    wstring name;        //贴图名称
    wstring fName;      //路径
};

class FRenderingTextureResourcesUpdate
    : public enable_shared_from_this<FRenderingTextureResourcesUpdate>
    , public IDirectXDeviceInterface
{
public:
    void LoadTextureResource(const wstring &inFileName);
    void BuildTextureConstantBuffer(ID3D12DescriptorHeap* inHead, int offset);
protected:
    std::unordered_map<std::wstring, std::unique_ptr<FRenderingTexture>> texturesMapping;
};