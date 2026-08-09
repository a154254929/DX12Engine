#ifndef SHADOW_FUNCTION_HLSL
#define SHADOW_FUNCTION_HLSL

#include "ShaderCommon.hlsl"

float GetShadowFactor(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    
    return ShadowMap.SampleCmpLevelZero(Shadow_Sampler, shadowTexUV.xy, shadowTexUV.z).r;
}

float GetShadowFactor_Direct(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float w = 2048.f;
    float sampleBiasSize = 1.f / w;
    
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    return ShadowMap.Sample(Point_Sampler, shadowTexUV.xy).r;
    
    //return ShadowMap.SampleCmpLevelZero(Shadow_Sampler, shadowTexUV.xy, shadowTexUV.z);
}

float GetShadowFactor_PCF_Sample4(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float w = 2048.f;
    float sampleBiasSize = 1.f / w;
    
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    float s1 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy).r;
    float s2 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy + float2(sampleBiasSize, 0)).r;
    float s3 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy + float2(0, sampleBiasSize)).r;
    float s4 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy + float2(sampleBiasSize, sampleBiasSize)).r;
    
    shadowTexUV.xyz /= shadowTexUV.w;
    
    float shadowDepth = shadowTexUV.z;
    float result1 = shadowDepth <= s1;
    float result2 = shadowDepth <= s2;
    float result3 = shadowDepth <= s3;
    float result4 = shadowDepth <= s4;
    
    float2 uv = frac(w * shadowTexUV.xy);
    
    return lerp(
        lerp(result1, result2, uv.x),
        lerp(result2, result3, uv.x), 
        uv.y
    ); 
}

float GetShadowFactor_PCF_Sample9(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    uint width = 0;
    uint height = 0;
    uint numMips = 0;
    
    ShadowMap.GetDimensions(0, width, height, numMips);
    float2 sampleBiasSize = float2(1.f / width, 1.f / height);
    
    const float2 constShadowSampleBias[9] = {
        float2(-sampleBiasSize.x, -sampleBiasSize.y),   float2(0, -sampleBiasSize.y),   float2(sampleBiasSize.x, -sampleBiasSize.y),
        float2(-sampleBiasSize.x, 0),                   float2(0, 0),                   float2(sampleBiasSize.x, 0),
        float2(-sampleBiasSize.x, sampleBiasSize.y),    float2(0, sampleBiasSize.y),    float2(sampleBiasSize.x, sampleBiasSize.y)
    };
    
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    shadowTexUV.xyz /= shadowTexUV.w;
    float result = 0.f;
    
    for (int i = 0; i < 9; i++)
    {
        result += ShadowMap.SampleCmpLevelZero(
            Shadow_Sampler,
            shadowTexUV.xy + constShadowSampleBias[i],
            shadowTexUV.z
        );
    }
    
    return max(result / 9.0f, 0.1f); 
}

float ProcessingImnidirectionalSampleCubeMapShadow(float4 inWorldPosition, float3 inLightPosition)
{
    float3 lightView = inWorldPosition.xyz - inLightPosition;
    
    float cubemapDepth = ShadowCubeMap[0].Sample(Point_Sampler, lightView).r;
    
    return cubemapDepth;
}

float ProcessingImnidirectionalSampleCmpLevelZeroShadow(float4 inWorldPosition, float3 inLightPosition)
{
    float3 lightView = inWorldPosition.xyz - inLightPosition;
    
    float cubemapDepth = ShadowCubeMap[0].SampleCmpLevelZero(Shadow_Sampler, float4(lightView, 1.f), 0.f).r;
    
    return cubemapDepth;
}
#endif
