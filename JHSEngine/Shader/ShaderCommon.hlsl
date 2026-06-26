
SamplerState Point_Sampler : register(s0);
SamplerState Anisotropic_Sampler : register(s1);
Texture2D Texture2DMap[Texture2DMap_Count] : register(t1);
TextureCube TextureCubeMap[CUBE_MAP_NUM] : register(t0);

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

cbuffer FogConstBuffer : register(b3)
{
    float3 FogColor;
    float FogStart;

    float FogRange;
    float FogHeight;
    float FogTransparentCoefficient;
}

struct MaterialConstBuffer
{
    uint MaterialType;
    float Roughness;
    int BaseColorIndex;
    int NormalMapIndex;
    
    int SpecularMapIndex;
    int RoughnessMapIndex;
    int ReversedMapIndex2;
    int ReversedMapIndex3;
    
    float4 BaseColor;
    
    float3 SpecularColor;
    int xx;
    
    float3 FresnelF0;
    float xx1;
    
    float4x4 Transformation;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t0, Space1);