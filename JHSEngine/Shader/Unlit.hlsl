#include "Light.hlsl"
#include "Material.hlsl"
#include "BRDF.hlsl"

cbuffer ObjectConstBuffer : register(b0) //b0->b14
{
    float4x4 WorldMatrix;
}

cbuffer ViewportConstBuffer : register(b1)
{
    float4 ViewportWorldPosition;
    float4x4 ViewProjectionMatrix;
}

cbuffer MaterialConstBuffer : register(b2)
{
    uint MaterialType;
    uint XX1;
    uint XX2;
    uint XX3;
    
    float4 BaseColor;
    float Roughness;
    float4x4 Transformation;
}

cbuffer LightConstBuffer : register(b3)
{
    float3 LightIntensity;
    float xx;
    float3 LightDirection;
    float xx1;
}


struct Varying
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 utangent : TANGENT;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 worldPosition : TEXCOORD1;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 worldNormal : TEXCOORD2;
    float3 utangent : TANGENT;
};

Attribute VertexShaderUnlit(Varying input)
{
    Attribute output;
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
    output.normal = input.normal;
    output.worldNormal = normalize(mul(input.normal, (float3x3) WorldMatrix));
    output.utangent = input.utangent;
    //output.normal = normalize(mul(input.normal, (float3x3) WorldMatrix)) * .5 + .5;
    //output.normal = input.normal;
    output.color = input.color;
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{
    //BaseColor
    if (MaterialType == 12) //phong
    {
        return input.color;
    }
    else if (MaterialType == 13) //phong
    {
        return float4(input.normal, 1.0f);
    }
    else if (MaterialType == 14) //phong
    {
        return float4(input.worldNormal, 1.0f);
    }
    float4 ambientLight = { .15f, .15f, .25f, 1.0f };
    
    FMaterial material;
    material.BaseColor = BaseColor;
    float3 normal = normalize(input.worldNormal.xyz);
    float3 lightDir = normalize(-LightDirection);
    float3 view = normalize((ViewportWorldPosition - input.worldPosition).xyz);
    
    float diffuse = 1.f;
    float nol = dot(lightDir, normal);
    float4 specularColor = 0;
    if (MaterialType == 0)  //lambertain
    {
        diffuse = saturate(nol);
    }
    else if (MaterialType == 1) //half-lambertain
    {
        float halfLambert = nol * .5f + .5f;
        diffuse = halfLambert;
    }
    else if (MaterialType == 2) //phong
    {
        diffuse = saturate(nol);
        
        float3 reflectLight = normalize(reflect(-lightDir, normal));
        float smoothness = 1.f - saturate(Roughness);
        float m = 100 * smoothness;
        specularColor.rgb = pow(saturate(dot(view, reflectLight)), m);
        specularColor.a = 1;
    }
    else if (MaterialType == 3) //blinn-phong
    {
        diffuse = saturate(nol);
        
        float3 halfView = normalize(view + lightDir);
        float smoothness = 1.f - saturate(Roughness);
        float m = 100 * smoothness;
        specularColor.rgb += nol > 0 ? pow(saturate(dot(normal, halfView)), m) : 0;
        specularColor.a = 1;

    }
    else if (MaterialType == 4) //WrapLight
    {
        //float wrapValue = 1.f; //lambertain
        float wrapValue = 2.f;
        diffuse = saturate((nol + wrapValue) / (1 + wrapValue));
    }
    else if (MaterialType == 5) //Minnaert
    {
        float smoothness = 1.f - saturate(Roughness);
        float r = 20 * smoothness;
        diffuse = saturate(nol) * pow(dot(normal, view) * saturate(nol), r);
    }
    else if (MaterialType == 6) //Bended
    {
        float diffuseLayer = 4.0f;
        diffuse = nol * .5f + .5f;
        diffuse = floor(diffuse * diffuseLayer) / diffuseLayer;
        
        float specularLayer = 2;
        float3 f0 = .1f;
        specularColor.rgb += FresnelSchlickMethod(f0, normal, view, 3).rgb;
        
        specularColor.rgb = floor(saturate(specularColor.rgb) * specularLayer) / specularLayer;
        
        specularColor.a = 1;
    }
    else if (MaterialType == 7) //Bended
    {
        
        float diffuseLayer = 4.0f;
        diffuse = nol * .5f + .5f;
        diffuse = floor(diffuse * diffuseLayer) / diffuseLayer;
        
        float specularLayer = 2;
        float3 f0 = .1f;
        specularColor.rgb += FresnelSchlickMethod(f0, normal, view, 3).rgb;
        
        specularColor.rgb = floor(saturate(specularColor.rgb) * specularLayer) / specularLayer;
        
        specularColor.a = 1;
        float4 color2 = float4(.4f, .5f, .8f, 1);
        material.BaseColor = lerp(color2, material.BaseColor, nol);
    }
    else if (MaterialType == 8) //Bended
    {
        float diffuseLayer = 4.0f;
        diffuse = nol * .5f + .5f;
        diffuse = floor(diffuse * diffuseLayer) / diffuseLayer;
        
        float specularLayer = 2;
        float3 f0 = .1f;
        specularColor.rgb += FresnelSchlickMethod(f0, normal, view, 3).rgb;
        
        float smoothness = 1.f - saturate(Roughness);
        float m = 100 * smoothness;
        //float3 halfView = normalize(view + lightDir);
        //specularColor.rgb += nol > 0 ? pow(saturate(dot(normal, halfView)), m) :0;
        float3 reflectLight = normalize(reflect(-lightDir, normal));
        specularColor.rgb += pow(saturate(dot(view, reflectLight)), m) * 31.25;
        
        specularColor.rgb = saturate(specularColor.rgb) * specularLayer;
        
        specularColor.a = 1;
    }
    else if (MaterialType == 9) //Back Light
    {
        float sssValue = .3f;
        diffuse = pow(saturate(dot(-normalize(normal * sssValue + lightDir), view)), 15.f);
        float wrapValue = 2.f;
        diffuse += saturate((nol + wrapValue) / (1 + wrapValue));
        
        float3 reflectLight = normalize(reflect(-lightDir, normal));
        float smoothness = 1.f - saturate(Roughness);
        float m = 100 * smoothness;
        specularColor.rgb = pow(saturate(dot(view, reflectLight)), m);
        specularColor.a = 1;

    }
    else if (MaterialType == 10) //AnisotropyKajiyaKay
    {
        

    }
    else if (MaterialType == 11) //OrenNayar
    {
        float nov = dot(normal, view);
        float phiri = length(view - normal * nov) * length(lightDir - normal * nol);
        
        float acosNOV = acos(nov);
        float acosNOL = acos(nol);
        
        float alpha = max(acosNOV, acosNOL);
        float beta = min(acosNOV, acosNOL);
        
        float roughness = pow(Roughness, 2);
        
        float a = 1 - .5f * (roughness / (roughness + .33f));
        float b = .45f * (roughness / (roughness + .09f));

        diffuse = saturate(nol) * (a + b * saturate(phiri) * sin(alpha) * tan(beta));

    }
    else if (MaterialType == 20) //PBR
    {
        float3 L = lightDir;
        float3 V = view;
        float3 H = normalize(V + L);
        float3 N = normal;
        float PI = acos(-1.0f);
        
        float Roughness = .2f;
        float Metallic = .2f;

        float4 D = GetDistributionGGX(N, H, Roughness);
        
        float3 F0 = 0.04;
        F0 = lerp(F0, material.BaseColor.rgb, Metallic);
        float4 F = float4(FresnelSchlickMethod(F0, N, V, 5), 1.0f);
        
        float G = GSmith(N, V, L, Roughness);
        
        float4 Kd = 1.0f - F;
        Kd *= 1.0f - Metallic;
        
        float3 Diffuse = Kd.rgb * GetDiffuseLambert(material.BaseColor.rgb);
        
        float NoV = saturate(dot(N, V));
        float NoL = nol;
        
        float4 Value = (D * G * F) / (4 * NoV * NoL);
        float4 Specalur = float4 (Value.rgb, 1.0f);
        
        float3 Radiance = LightIntensity;
        float3 finalColor = (Diffuse + Specalur.rgb) * NoL * Radiance;
        
        return float4(finalColor.rgb, 1.0f);
    }
    else if (MaterialType == 100) //Fresnel
    {
        float3 view = normalize((ViewportWorldPosition - input.worldPosition).xyz);
        float3 f0 = .1f;
        specularColor.rgb = FresnelSchlickMethod(f0, normal, view, 3).rgb;
    }
    
    return material.BaseColor * diffuse + material.BaseColor * ambientLight + material.BaseColor * specularColor;
    //return material.BaseColor;
    
    //return float4(input.normal, 1);
    //return float4(nol.xxx, 1);
}