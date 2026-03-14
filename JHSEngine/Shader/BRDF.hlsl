float4 GetDistributionGGX(float3 N, float3 H, float Roughness)
{
    float PI = acos(-1);
    float a2 = pow(Roughness, 4);
    float NoH = saturate(dot(N, H));
    float d = NoH * NoH * (a2 - 1) + 1;
    return a2 / max(PI * d * d, 1e-3);
}

float SchlickGGX(float Nov, float Roughness)
{
    float K_Dir = pow(Roughness, 2) * .125f;
    return Nov / max(Nov * (1 - K_Dir) + K_Dir, 1e-3);
}

float GSmith(float3 N, float3 V, float3 L, float Roughness)
{
    float NoV = saturate(dot(N, V));
    float NoL = saturate(dot(N, L));
    
    return SchlickGGX(NoV, Roughness) * SchlickGGX(NoL, Roughness);
}