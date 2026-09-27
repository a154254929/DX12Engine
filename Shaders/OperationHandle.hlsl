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
    
    /*
    float3 worldOriginPosition = mul(float4(0, 0, 0, 1), WorldMatrix).xyz;
    float3 viewDir = worldOriginPosition - ViewportWorldPosition.xyz;
    float3 worldDistance = length(viewDir);
    float3 scaledWorldOriginPosition = ViewportWorldPosition + viewDir * (100 / worldDistance);
    float4x4 newWorldMatrix = WorldMatrix;
    newWorldMatrix[3][0] = scaledWorldOriginPosition.x;
    newWorldMatrix[3][1] = scaledWorldOriginPosition.y;
    newWorldMatrix[3][2] = scaledWorldOriginPosition.z;
    output.worldPosition = mul(float4(input.position, 1), newWorldMatrix);
    */
    
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
    
    return output;
}

float4 PixelShaderOperationHandle(Attribute input) : SV_TARGET
{
    MaterialConstBuffer material = Materials[MaterialIndex];
    return material.BaseColor;
}