#include "Light.hlsl"
#include "ShaderCommon.hlsl"
#include "SkyFunction.hlsl"

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
    float4 worldPosition : TEXCOORD;
};


Attribute VertexShaderSky(Varying input)
{
    Attribute output = (Attribute)0;
    output.localPosition = float4(input.position, 1.0);
    
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
    
    return output;
}

float4 PixelShaderSky(Attribute input) : SV_TARGET
{
    float4 outColor = TextureCubeMap[0].Sample(Anisotropic_Sampler, input.localPosition);
    outColor = GetFogValue(outColor, input.worldPosition);
    return outColor;
    //return float4(.5, .5, .5, 1);
}