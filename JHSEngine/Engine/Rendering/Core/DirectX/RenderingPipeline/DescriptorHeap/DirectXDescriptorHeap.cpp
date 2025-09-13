#include "DirectXDescriptorHeap.h"

void FDirectXDescriptorHeap::Build(UINT inNumDescriptor)
{
    //构建栈
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    heapDesc.NumDescriptors = inNumDescriptor;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    heapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&cbvHeap));
}

void FDirectXDescriptorHeap::PreDraw(float deltaTime)
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { GetHeap() };
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void FDirectXDescriptorHeap::Draw(float deltaTime)
{

}

void FDirectXDescriptorHeap::PostDraw(float deltaTime)
{

}
