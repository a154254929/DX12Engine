#ifndef SHADOW_FUNCTION_HLSL
#define SHADOW_FUNCTION_HLSL

#include "ShaderCommon.hlsl"


float GetShadowFactor(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    
    return ShadowMap.SampleCmpLevelZero(Shadow_Sampler, shadowTexUV.xy, shadowTexUV.z);
}


#endif
