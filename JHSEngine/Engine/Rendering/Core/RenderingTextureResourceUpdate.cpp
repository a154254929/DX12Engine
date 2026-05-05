#include "RenderingTextureResourceUpdate.h"

void FRenderingTextureResourcesUpdate::LoadTextureResource(const wstring& inFileName)
{
        unique_ptr<FRenderingTexture> myTexture = std::make_unique<FRenderingTexture>();
        myTexture->fName = inFileName;
        
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
        
        texturesMapping[myTexture->name] = std::move(myTexture);
}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* inHead, int offset)
{
    
}
