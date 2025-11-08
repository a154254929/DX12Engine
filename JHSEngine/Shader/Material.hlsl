struct FMaterial
{
    float4 BaseColor;
};

float3 FresnelSchlickMethod(float3 inF0, float3 inNormal, float3 inView, float inM)
{
    return inF0 + pow((1.f - saturate(dot(inNormal, inView))), inM);
}