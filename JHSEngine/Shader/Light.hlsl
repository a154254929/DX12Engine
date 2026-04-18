#ifndef LIGHT_DEFINED
struct Light
{
    float3 LightIntensity;
    float xx;
    float3 LightDirection;
    float xx1;
};
#define LIGHT_DEFINED
#endif