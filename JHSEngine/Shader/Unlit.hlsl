cbuffer ConstBuffer : register(b0) //b0->b14
{
    float4x4 WorldMatrix;
}

cbuffer ViewportConstBuffer : register(b1)
{
    float4x4 ViewProjectionMatrix;
}

struct Varying
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

Attribute VertexShaderUnlit(Varying input)
{
    Attribute output;
    float4 worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(worldPosition, ViewProjectionMatrix);
    output.color = input.color;
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{
    return input.color;
}