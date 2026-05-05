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

void FDirectXRootSignature::BuildRootSignature(UINT inTextureNum)
{

    //构建根签名
    CD3DX12_ROOT_PARAMETER rootParam[5];

    //Object cbv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeObjCBV;
    descriptorRangeObjCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        0
    );

    //ViewPort cbv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeViewportCBV;
    descriptorRangeViewportCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        1
    );

    //Material cbv描述表
    // CD3DX12_DESCRIPTOR_RANGE descriptorRangeMaterialCBV;
    // descriptorRangeMaterialCBV.Init(
    //     D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
    //     1,
    //     2
    // );

    //Light cbv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeLightCBV;
    descriptorRangeLightCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        3
    );

    //Texture srv描述表
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeTextureSRV;
    descriptorRangeTextureSRV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
        inTextureNum,
        4
    );

    rootParam[0].InitAsDescriptorTable(1, &descriptorRangeObjCBV);
    rootParam[1].InitAsDescriptorTable(1, &descriptorRangeViewportCBV);
    //rootParam[2].InitAsDescriptorTable(1, &descriptorRangeMaterialCBV);
    rootParam[2].InitAsShaderResourceView(0, 1);
    rootParam[3].InitAsDescriptorTable(1, &descriptorRangeLightCBV);
    rootParam[4].InitAsDescriptorTable(1, &descriptorRangeTextureSRV);
    
    //静态采样方式
    std::vector<CD3DX12_STATIC_SAMPLER_DESC> samplerDescs;
    samplerDescs.push_back(
        CD3DX12_STATIC_SAMPLER_DESC(
            0,
            D3D12_FILTER_MIN_MAG_MIP_POINT
        )
    );

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        5,
        rootParam,
        samplerDescs.size(),    //数量
        samplerDescs.data(),    //实际指针
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