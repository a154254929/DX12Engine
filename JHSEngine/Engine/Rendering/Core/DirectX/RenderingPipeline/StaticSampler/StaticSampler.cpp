#include "StaticSampler.h"

FStaticSamplerObject::FStaticSamplerObject()
{
    
}

void FStaticSamplerObject::BuildStaticSample()
{
    samplerDescs.push_back(
        CD3DX12_STATIC_SAMPLER_DESC(
            0,
            D3D12_FILTER_MIN_MAG_MIP_POINT
        )
    );
    samplerDescs.push_back(
        CD3DX12_STATIC_SAMPLER_DESC(
            1,
            D3D12_FILTER_ANISOTROPIC,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            0,
            8
        )
    );
    samplerDescs.push_back(
        CD3DX12_STATIC_SAMPLER_DESC(
            2,
            D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
            D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            0,
            16,
            D3D12_COMPARISON_FUNC_LESS_EQUAL,
            D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE
        )
    );
}

CD3DX12_STATIC_SAMPLER_DESC* FStaticSamplerObject::GetData()
{
    return samplerDescs.data();
}

int FStaticSamplerObject::GetSize() const
{
    return samplerDescs.size();
}
