#include "Light.hlsl"
#include "Material.hlsl"

cbuffer ObjectConstBuffer : register(b0) //b0->b14
{
    float4x4 WorldMatrix;
}

cbuffer ViewportConstBuffer : register(b1)
{
    float4 ViewportWorldPosirion;
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
    float3 LightDirection;
}


struct Varying
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 worldPosition : TEXCOORD1;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

Attribute VertexShaderUnlit(Varying input)
{
    Attribute output;
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
    output.normal = normalize(mul(input.normal, (float3x3) WorldMatrix));
    //output.normal = normalize(mul(input.normal, (float3x3) WorldMatrix)) * .5 + .5;
    //output.normal = input.normal;
    output.color = input.color;
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{
    float4 ambientLight = { .15f, .15f, .25f, 1.0f };
    
    FMaterial material;
    material.BaseColor = BaseColor;
    float3 normal = normalize(input.normal);
    
    float diffusie = 1.f;
    float lambert = dot(normalize(-LightDirection), normal);
    float4 specularColor = 0;
    if (MaterialType == 0)  //lambertain
    {
        diffusie = saturate(lambert);
    }
    else if (MaterialType == 1) //half-lambertain
    {
        float halfLambert = lambert * .5f + .5f;
        diffusie = halfLambert;
    }
    else if (MaterialType == 2) //phong
    {
        diffusie = saturate(lambert);
        
        float smoothness = 1.f - saturate(Roughness);
        float3 view = normalize((ViewportWorldPosirion - input.worldPosition).xyz);
        float3 reflectLight = normalize(reflect(LightDirection, normal));
        float m = 100 * smoothness;
        specularColor.rgb = pow(saturate(dot(view, reflectLight)), m);
        specularColor.a = 1;

    }
    else if (MaterialType == 3) //blinn-phong
    {
        diffusie = saturate(lambert);
        
        float smoothness = 1.f - saturate(Roughness);
        float3 view = normalize((ViewportWorldPosirion - input.worldPosition).xyz);
        float3 halfView = normalize(view - LightDirection);
        float m = 100 * smoothness;
        specularColor.rgb = pow(saturate(dot(normal, halfView)), m);
        specularColor.a = 1;

    }
    else if (MaterialType == 4) //WrapLight
    {
        //float wrapValue = 1.f; //lambertain
        float wrapValue = 2.f;
        diffusie = saturate((lambert + wrapValue) / (1 + wrapValue));
    }
    else if (MaterialType == 5) //Minnaert
    {
        float smoothness = 1.f - saturate(Roughness);
        float r = 20 * smoothness;
        float3 view = normalize((ViewportWorldPosirion - input.worldPosition).xyz);
        diffusie = saturate(lambert) * pow(dot(normal, view) * saturate(lambert), r);
    }
    else if (MaterialType == 100) //Fresnel
    {
        float3 view = normalize((ViewportWorldPosirion - input.worldPosition).xyz);
        diffusie = pow(saturate(1 - dot(normal, view)), 1.5f);
    }
    
    return material.BaseColor * diffusie + material.BaseColor * ambientLight + material.BaseColor * specularColor;
    //return material.BaseColor;
    
    //return float4(input.normal, 1);
    //return float4(lambert.xxx, 1);
}