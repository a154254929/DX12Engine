#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FIMGUIPipeline : public IDirectXDeviceInterface_Struct
{
public:
    FIMGUIPipeline();
    
    void Init(ID3D12DescriptorHeap* inHeap, UINT inOffset);
    
    void Draw(float deltaTime);
    
    void Exit();
};