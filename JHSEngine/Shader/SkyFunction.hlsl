#ifndef SKY_FUNCTION_INCLUDED
#define SKY_FUNCTION_INCLUDED
float4 GetFogValue(float4 inColor, float3 inWorldPosition)
{
#if FOG_ON
    float viewLength = length(ViewportWorldPosition.xyz - inWorldPosition.xyz);
    float fogValue = saturate((viewLength - FogStart) / FogRange);
    inColor.rgb = lerp(inColor.rgb, FogColor.rgb, fogValue);
#endif
    return inColor;
}
#endif