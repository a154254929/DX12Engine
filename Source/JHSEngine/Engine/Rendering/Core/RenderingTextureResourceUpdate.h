#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterface.h"

struct FRenderingTexture
{
    FRenderingTexture()
        : renderingTextureID(0u)
    {
        
    }
    ComPtr<ID3D12Resource> uploadBuffer;
    ComPtr<ID3D12Resource> data;
    wstring name;               //贴图名称
    wstring fileName;           //路径
    wstring assetFileName;      //资源路径
    wstring simpleAssetFileName;      //资源路径
    UINT renderingTextureID;
};

class FRenderingTextureResourcesUpdate
    : public enable_shared_from_this<FRenderingTextureResourcesUpdate>
    , public IDirectXDeviceInterface
{
public:
    FRenderingTextureResourcesUpdate();
    
    void LoadTextureResource(const wstring &inFileName);
    
    void BuildTextureConstantBuffer(ID3D12DescriptorHeap* inHeap, int offset);
    
    void BuildParam();
    
    void ResetTextureByType(std::unique_ptr<FRenderingTexture>* inTexture);
    
    void SetViewDimension(D3D12_SRV_DIMENSION viewDimension);
    
    UINT Size() const{ return texturesMapping.size(); }
    
    std::unique_ptr<FRenderingTexture> *FindRenderingTextureByName(const std::string &inKey);
protected:
    std::map<std::wstring, std::unique_ptr<FRenderingTexture>> texturesMapping;
    
    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
};