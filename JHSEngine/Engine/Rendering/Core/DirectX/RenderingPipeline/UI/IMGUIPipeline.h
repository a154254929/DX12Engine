#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FIMGUIPipeline : public IDirectXDeviceInterface_Struct
{
public:
    FIMGUIPipeline();
    
    void Init(ID3D12DescriptorHeap* inHeap, UINT inOffset);
    
    void Draw(float deltaTime);
    
    void Exit();
    
protected:
    void Tick(float deltaTime);
    
private:
    bool bClicked = false;
    
    float sliderValue = 0.0f;
    
    ImVec4 imguiColor = ImVec4(1.f, 1.f, 1.f, 1.00f);
};