cbuffer ConstBuffer : register(b0)//b0->b14
{
    float4x4 World;
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
    output.position = mul(float4(input.position, 1), World);
    output.color = input.color;
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{
    return input.color;
}