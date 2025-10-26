cbuffer ObjectConstBuffer : register(b0) //b0->b14
{
    float4x4 WorldMatrix;
}

cbuffer ViewportConstBuffer : register(b1)
{
    float4x4 ViewProjectionMatrix;
}

cbuffer MaterialConstBuffer : register(b2)
{
    float4x4 Transformation;
}

cbuffer LightConstBuffer : register(b3)
{
    float3 LightIntensity;
    float3 LightDirection;
}


struct Varying
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

Attribute VertexShaderUnlit(Varying input)
{
    Attribute output;
    float4 worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(worldPosition, ViewProjectionMatrix);
    output.normal = normalize(mul(input.normal, (float3x3) WorldMatrix));
    output.color = input.color;
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{
    float lambert = saturate(dot(normalize(-LightDirection), normalize(input.normal)));
    return input.color * lambert;
    //return float4(input.normal, 1);
    //return float4(lambert.xxx, 1);
}