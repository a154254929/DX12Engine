#include "ConstantBufferView.h"

void FConstantBufferView::CreateConstant(UINT objectSize, UINT objectCount)
{
	constants = make_shared<FRenderingResourcesUpdate>();
    constants->Init(GetD3dDevice().Get(), objectSize, objectCount);
}

void FConstantBufferView::BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE inHandle, UINT inConstantBufferNum, UINT inHandleOffset)
{
    D3D12_GPU_VIRTUAL_ADDRESS objAddr = constants->GetBuffer()->GetGPUVirtualAddress();
    UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    for (int i = 0; i < inConstantBufferNum; ++i)
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle = inHandle;
        D3D12_CONSTANT_BUFFER_VIEW_DESC objCBVDesc;
        objCBVDesc.BufferLocation = objAddr + i * constants->GetConstantBufferByteSize();
        objCBVDesc.SizeInBytes = constants->GetConstantBufferByteSize();

        handle.Offset(i + inHandleOffset, descriptorOffset);
        GetD3dDevice()->CreateConstantBufferView(
            &objCBVDesc,
            handle
        );
    }
}

void FConstantBufferView::Update(int index, const void* inData)
{
    constants->Update(index, inData);
}
