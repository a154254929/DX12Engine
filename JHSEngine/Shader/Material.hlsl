
struct FMaterial
{
    float4 BaseColor;
    float Roughness;
};

float3 FresnelSchlickMethod(float3 inF0, float3 inNormal, float3 inView, float inM)
{
    return inF0 + pow((1.f - saturate(dot(inNormal, inView))), inM);
}

void GetMaterialBaseColor(MaterialConstBuffer inMaterialConst, float2 inTexcoord, inout FMaterial inoutMaterial)
{
    if(inMaterialConst.BaseColorIndex >= 0)
    {
        inoutMaterial.BaseColor = inMaterialConst.BaseColor * Texture2DMap[inMaterialConst.BaseColorIndex].Sample(Point_Sampler, inTexcoord);
    }
    else
    {
        inoutMaterial.BaseColor = inMaterialConst.BaseColor;
    }
}

void GetMaterialRoughness(MaterialConstBuffer inMaterialConst, float2 inTexcoord, inout FMaterial inoutMaterial)
{
    if(inMaterialConst.RoughnessMapIndex >= 0)
    {
        inoutMaterial.Roughness = Texture2DMap[inMaterialConst.RoughnessMapIndex].Sample(Point_Sampler, inTexcoord).r;
    }
    else
    {
        inoutMaterial.Roughness = inMaterialConst.Roughness;
    }
}

float3 GetMaterialNormal(
    MaterialConstBuffer inMaterialConst,
    float2 inTexcoord,
    float3 inNormal,
    float3 inTangent)
{
    float3 normal;
    if(inMaterialConst.NormalMapIndex >= 0)
    {
        float3 sampleNormal = Texture2DMap[inMaterialConst.NormalMapIndex].Sample(Anisotropic_Sampler, inTexcoord).rgb;
        sampleNormal = sampleNormal * 2.0f - 1.0f;
        float3x3 TBN = GetTBNMatrix(inNormal, inTangent);
        normal = mul(sampleNormal, TBN);
    }
    else
    {
        normal = inNormal;
    }
    return normal;
}

float3 GetMaterialSpecularColor(
    MaterialConstBuffer inMaterialConst,
    float2 inTexcoord
)
{
    if(inMaterialConst.SpecularMapIndex >= 0)
    {
        float4 specularColor = Texture2DMap[inMaterialConst.SpecularMapIndex].Sample(Anisotropic_Sampler, inTexcoord);
        return specularColor.rgb;
    }
    else
    {
        return inMaterialConst.SpecularColor;
    }
}

float3 GetReflectDir(float3 inUnitWorldNormal, float3 inViewDirection)
{
    return reflect(-inViewDirection, inUnitWorldNormal);
}

float3 GetReflectionSampleColor(float3 inUnitWorldNormal, float3 inDirection)
{
    return TextureCubeMap[0].Sample(Anisotropic_Sampler, inDirection).rgb;
}

float GetShininess(MaterialConstBuffer inMaterialConst)
{
    return 1.0 - inMaterialConst.Roughness;
}

float3 FresnelSchlickFactor(
    MaterialConstBuffer inMaterialConst,
    float3 inUnitWorldNormal,
    float3 inReflect
)
{
    return FresnelSchlickMethod(inMaterialConst.FresnelF0, inUnitWorldNormal, inReflect, 5);
}

float3 GetReflectionColor(
    MaterialConstBuffer inMaterialConst,
    float3 inUnitWorldNormal,
    float3 inViewDirection
)
{
    float3 reflectoinDir = GetReflectDir(inUnitWorldNormal, inViewDirection);
    float3 sampleRelctionColor = GetReflectionSampleColor(inUnitWorldNormal, reflectoinDir);
    float shininess = GetShininess(inMaterialConst);
    float3 fresnelFactor = FresnelSchlickFactor(inMaterialConst, inUnitWorldNormal, reflectoinDir);
    return sampleRelctionColor * shininess * fresnelFactor;
}