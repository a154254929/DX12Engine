#ifndef SKY_FUNCTION_INCLUDED
#define SKY_FUNCTION_INCLUDED
float4 GetFogValue(float4 inColor, float3 inWorldPosition)
{
#if FOG_ON
    float viewLength = length(ViewportWorldPosition.xyz - inWorldPosition.xyz);
    float fogValue = saturate((viewLength - FogStart) / FogRange);
    float heightFogValue = saturate((inWorldPosition.y - ViewportWorldPosition.y) / FogHeight);
    
    float3 fogColor = lerp(inColor.rgb, FogColor.rgb, fogValue);
    
    inColor.rgb = lerp(fogColor.rgb, inColor.rgb, heightFogValue);
#endif
    return inColor;
}
#endif