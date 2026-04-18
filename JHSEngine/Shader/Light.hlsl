#ifndef LIGHT_DEFINED
struct Light
{
    float3 LightIntensity;
    float StartAttenuation;
    float3 LightDirection;
    float EndAttenuation;
    float3 LightPosition;
    int LightType;
};
#define LIGHT_DEFINED
#endif