#include "RenderingTextureResourceUpdate.h"

void FRenderingTextureResourcesUpdate::LoadTextureResource(const wstring& inFileName)
{
        unique_ptr<FRenderingTexture> myTexture = std::make_unique<FRenderingTexture>();
        myTexture->fileName = inFileName;
        
        wchar_t fileName[1024] = {0};
        get_path_clean_filename_w(fileName, inFileName.c_str());
        myTexture->name = fileName;
        
        //读取DDS数据
        CreateDDSTextureFromFile12(
            GetD3dDevice().Get(),
            GetGraphicsCommandList().Get(),
            inFileName.c_str(),
            myTexture->data,
            myTexture->uploadBuffer
        );
        myTexture->renderingTextureID = texturesMapping.size() + 1;
        
        texturesMapping[myTexture->name] = std::move(myTexture);
}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* inHeap, int offset)
{
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(inHeap->GetCPUDescriptorHandleForHeapStart());
    handle.Offset(offset, descriptorOffset);
    
    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;
    
    
    for (auto &tmp : texturesMapping)
    {
        shaderResourceViewDesc.Format = tmp.second->data->GetDesc().Format;
        shaderResourceViewDesc.Texture2D.MipLevels = tmp.second->data->GetDesc().MipLevels;
        GetD3dDevice()->CreateShaderResourceView(
            tmp.second->data.Get(),
            &shaderResourceViewDesc,
            handle
        );
        handle.Offset(1, descriptorOffset);
    }
    
}

std::unique_ptr<FRenderingTexture>* FRenderingTextureResourcesUpdate::FindRenderingTextureByName(const std::string& inKey)
{
    if (!inKey.empty())
    {
        const char* InString = inKey.c_str();
        wchar_t texturePath[1024] = {0};
        char_to_wchar_t(texturePath, 1024, InString);
        if (texturesMapping.find(texturePath) != texturesMapping.end())
        {
            return &texturesMapping[texturePath];
        }
        else
        {
            for (auto &tmp : texturesMapping)
            {
                if (tmp.second->fileName != texturePath)
                {
                    return &tmp.second;
                }
                if (tmp.second->assetFileName == texturePath)
                {
                    return &tmp.second;
                }
                if (tmp.second->simpleAssetFileName == texturePath)
                {
                    return &tmp.second;
                }
            }
        }
    }
    return nullptr;
}
