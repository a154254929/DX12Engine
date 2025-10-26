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

void FDirectXRootSignature::BuildRootSignature()
{

    //构建根签名
    CD3DX12_ROOT_PARAMETER rootParam[2];

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
    CD3DX12_DESCRIPTOR_RANGE descriptorRangeViewportCBV;
    descriptorRangeViewportCBV.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1,
        2
    );

    rootParam[0].InitAsDescriptorTable(1, &descriptorRangeObjCBV);
    rootParam[1].InitAsDescriptorTable(1, &descriptorRangeViewportCBV);
    rootParam[2].InitAsDescriptorTable(1, &descriptorRangeViewportCBV);

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        3,
        rootParam,
        0,
        nullptr,
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