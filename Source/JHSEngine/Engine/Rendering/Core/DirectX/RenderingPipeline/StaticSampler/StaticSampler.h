#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

struct FStaticSamplerObject : public IDirectXDeviceInterface_Struct
{
    FStaticSamplerObject();
    
    void BuildStaticSample();
    
    CD3DX12_STATIC_SAMPLER_DESC* GetData();
    int GetSize() const;
    
    //静态采样方式
    std::vector<CD3DX12_STATIC_SAMPLER_DESC> samplerDescs;
};