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
    CD3DX12_ROOT_PARAMETER rootParam[9];


    //TextureCubemap srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeTextureCubemapSRV;
    UINT textureCubemapNum = inTextureCubemapNum > 0 ? inTextureCubemapNum : 1;
    descriptorRangeTextureCubemapSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        textureCubemapNum,
        0
    );
    
    //shadowMap srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeShadowMapSRV;
    descriptorRangeShadowMapSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        1
    );
    
    //shadowCubeMap srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeShadowCubeMapSRV;
    descriptorRangeShadowCubeMapSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        1,
        2
    );

    //Texture2D srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeTexture2DSRV;
    descriptorRangeTexture2DSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        inTextureNum,
        3
    );

    rootParam[0].InitAsConstantBufferView(0);   //对象
    rootParam[1].InitAsConstantBufferView(1);   //视口
    rootParam[2].InitAsConstantBufferView(2);   //灯光
    rootParam[3].InitAsConstantBufferView(3);   //雾
    
    //t
    rootParam[4].InitAsShaderResourceView(0, 1);    //材质
    
    /***********************贴图************************/
    //2D贴图
    rootParam[5].InitAsDescriptorTable(1, &descriptorRangeTexture2DSRV, D3D12_SHADER_VISIBILITY_ALL);
    //Cubemap贴图
    rootParam[6].InitAsDescriptorTable(1, &descriptorRangeTextureCubemapSRV, D3D12_SHADER_VISIBILITY_ALL);
    //shadowMap
    rootParam[7].InitAsDescriptorTable(1, &descriptorRangeShadowMapSRV, D3D12_SHADER_VISIBILITY_ALL);
    //shadowCubeMap
    rootParam[8].InitAsDescriptorTable(1, &descriptorRangeShadowCubeMapSRV, D3D12_SHADER_VISIBILITY_ALL);
    
    //构建静态采样
    staticSamplerObject.BuildStaticSample();

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        9,
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
