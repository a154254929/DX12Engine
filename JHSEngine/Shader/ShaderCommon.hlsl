
SamplerState Texture2DMap_Sampler : register(s0);
Texture2D Texture2DMap[Texture2DMap_Count] : register(t3);

cbuffer ObjectConstBuffer : register(b0) //b0->b14
{
    float4x4 WorldMatrix;
    float4x4 ObjectTextureTransform;
    uint MaterialIndex;
    uint RR0;
    uint RR1;
    uint RR2;
}

cbuffer ViewportConstBuffer : register(b1)
{
    float4 ViewportWorldPosition;
    float4x4 ViewProjectionMatrix;
}

cbuffer LightConstBuffer : register(b2)
{
    int4 LightInfo;
    Light SceneLights[16];
}

struct MaterialConstBuffer
{
    uint MaterialType;
    float Roughness;
    int BaseColorIndex;
    int NormalMapIndex;
    
    float4 BaseColor;
    float4x4 Transformation;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t4, space1);