#include "Light.hlsl"
#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"
#include "Material.hlsl"

struct Varying
{
    float3 position : POSITION;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 localPosition : POSITION;
    float4 worldPosition : TEXCOORD;
};

Attribute VertexShaderOperationHandle(Varying input)
{
    Attribute output = (Attribute)0;
    output.localPosition = float4(input.position, 1.0);
    
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
    
    return output;
}

float4 PixelShaderOperationHandle(Attribute input) : SV_TARGET
{
    MaterialConstBuffer material = Materials[MaterialIndex];
    return material.BaseColor;
}