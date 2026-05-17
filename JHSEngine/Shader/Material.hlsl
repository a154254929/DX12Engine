
struct FMaterial
{
    float4 BaseColor;
};

float3 FresnelSchlickMethod(float3 inF0, float3 inNormal, float3 inView, float inM)
{
    return inF0 + pow((1.f - saturate(dot(inNormal, inView))), inM);
}

void GetMaterialBaseColor(MaterialConstBuffer materialConst, float2 inTexcoord, inout FMaterial inoutMaterial)
{
    if(materialConst.BaseColorIndex >= 0)
    {
        inoutMaterial.BaseColor = materialConst.BaseColor * Texture2DMap[materialConst.BaseColorIndex].Sample(Point_Sampler, inTexcoord);
    }
    else
    {
        inoutMaterial.BaseColor = materialConst.BaseColor;
    }
}

float3 GetMaterialNormal(
    MaterialConstBuffer materialConst,
    float2 inTexcoord,
    float3 inNormal,
    float3 inTangent)
{
    float3 normal;
    if(materialConst.NormalMapIndex >= 0)
    {
        float3 sampleNormal = Texture2DMap[materialConst.NormalMapIndex].Sample(Anisotropic_Sampler, inTexcoord);
        sampleNormal = sampleNormal * 2.0f - 1.0f;
        float3x3 TBN = GetTBNMatrix(inNormal, inTangent);
        normal = mul(sampleNormal, TBN);
    }
    else
    {
        normal = inNormal;
    }
    return normalize(mul(normal, (float3x3) WorldMatrix));
}

float3 GetMaterialSpecularColor(
    MaterialConstBuffer materialConst,
    float2 inTexcoord)
{
    if(materialConst.SpecularMapIndex >= 0)
    {
        float4 specularColor = Texture2DMap[materialConst.SpecularMapIndex].Sample(Anisotropic_Sampler, inTexcoord);
        return specularColor.rgb;
    }
    else
    {
        return materialConst.SpecularColor;
    }
}