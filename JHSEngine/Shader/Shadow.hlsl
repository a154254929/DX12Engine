#include "Light.hlsl"
#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"
#include "Material.hlsl"

struct Varying
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Attribute VertexShaderUnlit(Varying input)
{
    Attribute output;
    float4 worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(worldPosition, ViewProjectionMatrix);
	
    //uv坐标
    float4 myTexcoord = mul(float4(input.texcoord, 0.f, 1.0f), ObjectTextureTransform);
    output.uv = myTexcoord.xy;
    return output;
}

void PixelShaderUnlit(Attribute input)
{

}