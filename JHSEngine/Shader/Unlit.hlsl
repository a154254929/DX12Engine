#include "Light.hlsl"
#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"
#include "Material.hlsl"
#include "BRDF.hlsl"
#include "SkyFunction.hlsl"


struct Varying
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 utangent : TANGENT;
    float3 vtangent : BITANGENT;
    float2 texcoord : TEXCOORD;
};

struct Attribute
{
    float4 position : SV_POSITION;
    float4 worldPosition : TEXCOORD1;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 utangent : TANGENT;
    float2 uv : TEXCOORD;
};

Attribute VertexShaderUnlit(Varying input)
{
    Attribute output;
    output.worldPosition = mul(float4(input.position, 1), WorldMatrix);
    output.position = mul(output.worldPosition, ViewProjectionMatrix);
    output.normal = input.normal;
    output.utangent = input.utangent;
    //output.normal = normalize(mul(input.normal, (float3x3) WorldMatrix)) * .5 + .5;
    //output.normal = input.normal;
    output.color = input.color;
	
	//uv坐标
	float4 myTexcoord = mul(float4(input.texcoord, 0.f, 1.0f), ObjectTextureTransform);
	output.uv = myTexcoord.xy;
    return output;
}

float4 PixelShaderUnlit(Attribute input) : SV_TARGET
{

	MaterialConstBuffer materialConst = Materials[MaterialIndex];

    //BaseColor
    float4 ambientLight = { .15f, .15f, .25f, 1.0f };
    
    FMaterial material = (FMaterial)0;
	GetMaterialBaseColor(materialConst, input.uv, material);
	GetMaterialRoughness(materialConst, input.uv, material);
    float3 normal = normalize(input.normal.xyz);
	normal = GetMaterialNormal(materialConst, input.uv, normal, input.utangent);
	float3 worldNormal = normalize(mul(normal, (float3x3) WorldMatrix));
    if (materialConst.MaterialType == 12) //BaseColor
    {
        return material.BaseColor;
    }
	else if (materialConst.MaterialType == 13) //Normal
    {
        return float4(input.normal, 1.0f);
    }
    else if (materialConst.MaterialType == 14) //WorldNormal
    {
        return float4(worldNormal, 1.0f);
    }
	
    float3 view = normalize((ViewportWorldPosition - input.worldPosition).xyz);
    float4 lightStrengths = { 0.f,0.f,0.f,1.f };
    float4 specularColor = 0;
    //float4 specularColor = 1;
	float3 specular = GetMaterialSpecularColor(materialConst, input.uv);
	//return float4(specular, 1);
    for (int i = 0; i < LightInfo.x; i++)
    {
        float diffuse = 0.f;
        float4 lightStrength = ComputeLightStrength(SceneLights[i], worldNormal, input.worldPosition.xyz);
        float3 lightDir = normalize(GetLightDirection(SceneLights[i], input.worldPosition.xyz));
    
        float nol = dot(lightDir, worldNormal);
        if (materialConst.MaterialType == 0)  //lambertain
        {
            //diffuse += pow(saturate(nol), 2.0f);
            diffuse += saturate(nol);
        }
        else if (materialConst.MaterialType == 1) //half-lambertain
        {
            float halfLambert = nol * .5f + .5f;
            diffuse += halfLambert;
        }
        else if (materialConst.MaterialType == 2) //phong
        {
            diffuse += pow(saturate(nol), 2.0f);
            
            float3 reflectLight = normalize(reflect(-lightDir, worldNormal));
            float smoothness = 1.f - saturate(material.Roughness);
            float m = 100 * smoothness;
            specularColor.rgb += saturate(specular * (m + 2.0f) * pow(saturate(dot(view, reflectLight)), m) / (acos(-1.0f) * 2.0f));
            specularColor.a = 1;
        }
        else if (materialConst.MaterialType == 3) //blinn-phong
        {
            diffuse += pow(saturate(nol), 2.0f);
            
            float3 halfView = normalize(view + lightDir);
            float smoothness = 1.f - saturate(material.Roughness);
            float m = 100 * smoothness;
            specularColor.rgb += saturate(specular * (m + 2.0f) * pow(saturate(dot(worldNormal, halfView)), m) / (acos(-1.0f) * 2.0f));
            specularColor.a = 1;

        }
        else if (materialConst.MaterialType == 4) //WrapLight
        {
            //float wrapValue = 1.f; //lambertain
            float wrapValue = 2.f;
            diffuse += saturate((nol + wrapValue) / (1 + wrapValue));
        }
        else if (materialConst.MaterialType == 5) //Minnaert
        {
            float smoothness = 1.f - saturate(material.Roughness);
            float r = 20 * smoothness;
            diffuse += pow(saturate(nol), 2.0f) * pow(dot(worldNormal, view) * pow(saturate(nol), 2.0f), r);
        }
        else if (materialConst.MaterialType == 6) //Bended
        {
            float diffuseLayer = 4.0f;
            diffuse += floor((nol * .5f + .5f) * diffuseLayer) / diffuseLayer;
            
            float specularLayer = 2;
            float3 f0 = .1f;
            
            specularColor.rgb += saturate(specular * floor(saturate(FresnelSchlickMethod(f0, worldNormal, view, 3).rgb) * specularLayer) / specularLayer);
            
            specularColor.a = 1;
        }
        else if (materialConst.MaterialType == 7) //Bended
        {
            
            float diffuseLayer = 4.0f;
            diffuse += floor((nol * .5f + .5f) * diffuseLayer) / diffuseLayer;
            
            float specularLayer = 2;
            float3 f0 = .1f;
            
            specularColor.rgb += saturate(specular * floor(saturate(FresnelSchlickMethod(f0, worldNormal, view, 3).rgb) * specularLayer) / specularLayer);
            
            specularColor.a = 1;
            float4 color2 = float4(.4f, .5f, .8f, 1);
            material.BaseColor = lerp(color2, material.BaseColor, nol);
        }
        else if (materialConst.MaterialType == 8) //Bended
        {
            float diffuseLayer = 4.0f;
            diffuse += floor((nol * .5f + .5f) * diffuseLayer) / diffuseLayer;
            
            float specularLayer = 2;
            float3 f0 = .1f;
            
            float smoothness = 1.f - saturate(material.Roughness);
            float m = 100 * smoothness;
            //float3 halfView = normalize(view + lightDir);
            //specularColor.rgb += nol > 0 ? saturate(pow(saturate(dot(worldNormal, halfView)), m)) :0;
            float3 reflectLight = normalize(reflect(-lightDir, worldNormal));
            
            specularColor.rgb = saturate(specular * saturate((pow(saturate(dot(view, reflectLight)), m) * 31.25)) * specularLayer);
            
            specularColor.a = 1;
        }
        else if (materialConst.MaterialType == 9) //Back Light
        {
            float sssValue = .3f;
            diffuse = pow(saturate(dot(-normalize(worldNormal * sssValue + lightDir), view)), 15.f);
            float wrapValue = 2.f;
            diffuse += saturate((nol + wrapValue) / (1 + wrapValue));
            
            float3 reflectLight = normalize(reflect(-lightDir, worldNormal));
            float smoothness = 1.f - saturate(material.Roughness);
            float m = 100 * smoothness;
            specularColor.rgb += saturate(specular * pow(saturate(dot(view, reflectLight)), m));
            specularColor.a = 1;

        }
        else if (materialConst.MaterialType == 10) //AnisotropyKajiyaKay
        {
            

        }
        else if (materialConst.MaterialType == 11) //OrenNayar
        {
            float nov = dot(worldNormal, view);
            float phiri = length(view - worldNormal * nov) * length(lightDir - worldNormal * nol);
            
            float acosNOV = acos(nov);
            float acosNOL = acos(nol);
            
            float alpha = max(acosNOV, acosNOL);
            float beta = min(acosNOV, acosNOL);
            
            float roughness = pow(material.Roughness, 2);
            
            float a = 1 - .5f * (roughness / (roughness + .33f));
            float b = .45f * (roughness / (roughness + .09f));

            diffuse += pow(saturate(nol), 2.0f) * (a + b * saturate(phiri) * sin(alpha) * tan(beta));

        }
        /*
        else if (materialConst.MaterialType == 20) //PBR
        {
            float3 L = lightDir;
            float3 V = view;
            float3 H = normalize(V + L);
            float3 N = worldNormal;
            float PI = acos(-1.0f);
            
            float roughness = .2f;
            float Metallic = .2f;

            float4 D = GetDistributionGGX(N, H, roughness);
            
            float3 F0 = 0.04;
            F0 = lerp(F0, material.BaseColor.rgb, Metallic);
            float4 F = float4(FresnelSchlickMethod(F0, N, V, 5), 1.0f);
            
            float G = GSmith(N, V, L, roughness);
            
            float4 Kd = 1.0f - F;
            Kd *= 1.0f - Metallic;
            
            float3 Diffuse = Kd.rgb * GetDiffuseLambert(material.BaseColor.rgb);
            
            float NoV = saturate(dot(N, V));
            float NoL = nol;
            
            float4 Value = (D * G * F) / (4 * NoV * NoL);
            float4 Specalur = float4 (Value.rgb, 1.0f);
            
            float3 Radiance = lightIntensity;
            float3 finalColor = (Diffuse + Specalur.rgb) * NoL * Radiance;
            
            return float4(finalColor.rgb, 1.0f);
        }
        */
        else if (materialConst.MaterialType == 100) //Fresnel
        {
            float3 view = normalize((ViewportWorldPosition - input.worldPosition).xyz);
            float3 f0 = .1f;
            specularColor.rgb = saturate(specular * FresnelSchlickMethod(f0, worldNormal, view, 3).rgb);
            break;
        }
        
        lightStrengths += lightStrength * diffuse * float4(SceneLights[i].LightIntensity,1.f);
    }

	lightStrengths = saturate(lightStrengths);
	material.BaseColor = saturate(material.BaseColor);
	specularColor = saturate(specularColor);
    
    float4 outColor = lightStrengths * (
		material.BaseColor
		+ material.BaseColor * specularColor)
    + material.BaseColor * ambientLight;

	//计算反射
	switch(materialConst.MaterialType)
	{
		case 2:
		case 3:
		case 9:
			float3 reflectionColor = GetReflectionColor(materialConst, worldNormal, view);
			outColor.rgb += reflectionColor;
		break;	
	}

	outColor.a = material.BaseColor.a;
    
	//计算雾
    outColor = GetFogValue(outColor, input.worldPosition);
    
    return outColor;
    //return material.BaseColor;
    
    //return float4(input.worldNormal, 1);
    //return float4(nol.xxx, 1);
}