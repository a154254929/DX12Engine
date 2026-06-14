#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{
}

void FDirectXRootSignature::PreDraw(float deltaTime)
{
    GetGraphicsCommandList()->SetGraphicsRootSignature(GetRootSignature());
}

void FDirectXRootSignature::Draw(float deltaTime)
{
}

void FDirectXRootSignature::PostDraw(float deltaTime)
{
}

void FDirectXRootSignature::BuildRootSignature(UINT inTextureNum, UINT inTextureCubemapNum)
{

    //构建根签名
    CD3DX12_ROOT_PARAMETER rootParam[6];

    //TextureCubemap srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeTextureCubemapSRV;
    descriptorRangeTextureCubemapSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        inTextureCubemapNum,
        0
    );

    //Texture2D srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeTexture2DSRV;
    descriptorRangeTexture2DSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        inTextureNum,
        1
    );

    rootParam[0].InitAsConstantBufferView(0);   //对象
    rootParam[1].InitAsConstantBufferView(1);   //视口
    rootParam[2].InitAsConstantBufferView(2);   //灯光
    
    //t
    rootParam[3].InitAsShaderResourceView(0, 1);    //材质
    
    rootParam[4].InitAsDescriptorTable(1, &descriptorRangeTexture2DSRV, D3D12_SHADER_VISIBILITY_ALL);//贴图
    rootParam[5].InitAsDescriptorTable(1, &descriptorRangeTextureCubemapSRV, D3D12_SHADER_VISIBILITY_ALL);//贴图
    
    //构建静态采样
    staticSamplerObject.BuildStaticSample();

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        6,
        rootParam,
        staticSamplerObject.GetSize(),    //数量
        staticSamplerObject.GetData(),    //实际指针
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    //创建
    ComPtr<ID3DBlob> serializeRootSignature;
    ComPtr<ID3DBlob> errorBlob;
    D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        serializeRootSignature.GetAddressOf(),
        errorBlob.GetAddressOf()
    );
    if (errorBlob)
    {
        Engine_Log_Error("%s", (char*)errorBlob->GetBufferPointer());
    }
    //创建
    GetD3dDevice()->CreateRootSignature(
        0,
        serializeRootSignature->GetBufferPointer(),
        serializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature)
    );
}