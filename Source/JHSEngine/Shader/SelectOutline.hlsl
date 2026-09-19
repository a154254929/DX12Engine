#include "Light.hlsl"
#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"
#include "Material.hlsl"

struct Varying
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 utangent : TANGENT;
    float3 vtangent : BITANGENT;
    float2 texcoord : TEXCOORD;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 worldPosition : TEXCOORD0;
    float3 worldNormal : TEXCOORD1;
    float2 uv : TEXCOORD2;
};

Attribute SelectOutlineVertexShader(Varying input)
{
    Attribute output;
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
	
    //uv坐标
    float4 myTexcoord = mul(float4(input.texcoord, 0.f, 1.0f), ObjectTextureTransform);
    output.uv = myTexcoord.xy;
    output.worldNormal = normalize(mul(input.normal, (float3x3)WorldMatrix));
    output.position.z = output.position.z - 0.0001f;
    return output;
}

float4 SelectOutlinePixelShader(Attribute input) : SV_TARGET
{
    float3 viewDirection = normalize(ViewportWorldPosition - input.worldPosition.xyz);
    float3 worldNormal = normalize(input.worldNormal.xyz);

    return float4(.6f, .3f, 0.f, 1.f) * smoothstep(.6f, .8f, (1.0f - dot(worldNormal, viewDirection)));
    //return float4(worldNormal, 1.f) ;
}