#ifndef LIGHT_DEFINED
struct Light
{
    float3 LightIntensity;
    float StartAttenuation;
    
    float3 LightDirection;
    float EndAttenuation;
    
    float3 LightPosition;
    int LightType;
    
    float ConicalInnerCorner; //for spot light
    float ConicalOuterCorner; //for spot light
    float2 xx;
};

float3 GetLightDirection(Light light, float3 inWorldPosition)
{
    if (light.LightType == 1)       //Parallel
    {
        return -light.LightDirection;
    }
    else if (light.LightType == 2)  //Point
    {
        return light.LightPosition - inWorldPosition;
    }
    else if (light.LightType == 3)  //Spot
    {
        return light.LightPosition - inWorldPosition;
    }
    
    return 0;
}

float4 AttenuationPointLights1(Light light, float distance)
{
    float4 lightStrength = 1.0f;
    float4 attenuationRange = light.EndAttenuation - light.StartAttenuation;
    return lightStrength * (1.0 - distance / attenuationRange);
}

float4 AttenuationPointLights2(Light light, float distance, float c, float i, float q)
{
    float4 lightStrength = 1.0f;
    return (1.0f / (c + i * distance + pow(q, 2.0f) * distance)) * lightStrength;
}

float4 ComputeLightStrength(Light light, float3 inWorldNormal, float3 inWorldPosition )
{
    if (light.LightType == 1)       //Parallel
    {
        return 1.0f;
    }
    else if (light.LightType == 2)  //Point
    {
        float4 lightStrength = 1.0f;
        float3 lightVector = light.LightPosition - inWorldPosition;
        float distance = length(lightVector);
        
        if (distance < light.EndAttenuation)
        {
            return AttenuationPointLights1(light, distance) * lightStrength;
            //return AttenuationPointLights2(light, distance) * lightStrength;
        }
    }
    else if (light.LightType == 3)  //Spot
    {
        float3 lightVector = light.LightPosition - inWorldPosition;
        float4 lightStrength = pow(saturate(dot(light.LightDirection, normalize(lightVector))), 1.f);
        float distance = length(lightVector);
        
        if (distance < light.EndAttenuation)
        {
            return AttenuationPointLights1(light, distance) * lightStrength;
            //return AttenuationPointLights2(light, distance, 0.f, 0.7f, 0.9f) * lightStrength;
        }
    }
    
    return float4( 0.f,0.f,0.f,1.f );
}
#define LIGHT_DEFINED
#endif