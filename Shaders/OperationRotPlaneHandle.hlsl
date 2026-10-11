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
    float4 localPosition : POSITION;
    float4 worldPosition : TEXCOORD;
    float2 uv : TEXCOORD1;
};

Attribute VertexShaderOperationRotPlaneHandle(Varying input)
{
    MaterialConstBuffer material = Materials[MaterialIndex];
    
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
    float4 texcoord = mul(float4(input.texcoord, 0, 1), ObjectTextureTransform);
    output.uv = mul(texcoord, material.Transformation).xy;
    
    return output;
}

float4 PixelShaderOperationRotPlaneHandle(Attribute input) : SV_TARGET
{
    MaterialConstBuffer material = Materials[MaterialIndex];
    return float4(material.BaseColor.rgb, .5f);
}