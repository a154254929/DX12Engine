#include "ConstantBufferView.h"

void FConstantBufferView::CreateConstant(UINT objectSize, UINT objectCount)
{
	constants = make_shared<FRenderingResourcesUpdate>();
    constants->Init(GetD3dDevice().Get(), objectSize, objectCount);
}

void FConstantBufferView::BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE inHandle, UINT inCinstantBufferNum, UINT inHandeOffset = 0)
{
    D3D12_GPU_VIRTUAL_ADDRESS objAddr = constants.get()->GetBuffer()->GetGPUVirtualAddress();
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    for (int i = 0; i < inCinstantBufferNum; ++i)
    {
        D3D12_CONSTANT_BUFFER_VIEW_DESC objCBVDesc;
        objCBVDesc.BufferLocation = objAddr + constants->GetConstantBufferByteSize();
        objCBVDesc.SizeInBytes = constants->GetConstantBufferByteSize();

        inHandle.Offset(i + inHandeOffset, descriptorOffset);
        GetD3dDevice()->CreateConstantBufferView(
            &objCBVDesc,
            inHandle
        );
    }
}

void FConstantBufferView::Update(int index, const void* inData)
{
    constants->Update(index, inData);
}
