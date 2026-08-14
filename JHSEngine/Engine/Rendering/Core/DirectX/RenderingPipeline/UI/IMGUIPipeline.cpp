#include "IMGUIPipeline.h"

FIMGUIPipeline::FIMGUIPipeline()
{
    
}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* inHeap, UINT inOffset)
{
    IMGUI_CHECKVERSION();
    
    ImGui::CreateContext();
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplWin32_Init(GetMainWindowsHandle());
    
    UINT cbvDesSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
    auto cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(
        inHeap->GetCPUDescriptorHandleForHeapStart(),
        inOffset,
        cbvDesSize
    );
    auto gpuDescriptor = CD3DX12_GPU_DESCRIPTOR_HANDLE(
        inHeap->GetGPUDescriptorHandleForHeapStart(),
        inOffset,
        cbvDesSize
    );
    
    ImGui_ImplDX12_Init(
        GetD3dDevice().Get(),
        3,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        inHeap,
        cpuDescriptor,
        gpuDescriptor
    );
}

void FIMGUIPipeline::Draw(float deltaTime)
{
    ImGui_ImplDX12_NewFrame();;
    ImGui_ImplWin32_NewFrame();
    
    //绘制
    Tick(deltaTime);
    
    ImGui::Render();
    
    ImGui_ImplDX12_RenderDrawData(
        ImGui::GetDrawData(),
        GetGraphicsCommandList().Get()
    );
}

void FIMGUIPipeline::Exit()
{
}

void FIMGUIPipeline::Tick(float deltaTime)
{
}
