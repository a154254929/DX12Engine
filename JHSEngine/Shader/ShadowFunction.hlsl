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


float GetShadowFactor_PCF(float4 inWorldPosition, float4x4 inShadowMatrix)
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
    
    //return 
}


#endif
