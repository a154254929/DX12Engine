float4 GetDistributionGGX(float3 N, float3 H, float Roughness)
{
    float PI = acos(-1);
    float a2 = pow(Roughness, 4);
    float N0H = saturate(dot(N, H));
    float d = N0H * N0H * (a2 - 1) + 1;
    return a2 / max(PI * d * d, 1e-3);
}