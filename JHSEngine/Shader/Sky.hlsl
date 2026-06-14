#include "Light.hlsl"
#include "ShaderCommon.hlsl"

struct Varying
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 localPosition : POSITION;
};


Attribute VertexShaderUnlit(Varying input)
{
    Attribute output = (Attribute)0;
    output.localPosition = float4(input.position, 1.0);
    
    float4 worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(worldPosition, ViewProjectionMatrix);
    
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{
    return TextureCubeMap.Sample(Anisotropic_Sampler, input.localPosition);
    //return float4(.5, .5, .5, 1);
}