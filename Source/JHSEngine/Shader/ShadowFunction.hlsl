#ifndef SHADOW_FUNCTION_HLSL
#define SHADOW_FUNCTION_HLSL

#include "ShaderCommon.hlsl"

#define PI 3.1415926535897932384626433832795f
#define PI_DIV4 0.78539816339744830961566084581988f

float GetShadowFactor(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    
    return ShadowMap.SampleCmpLevelZero(Shadow_Sampler, shadowTexUV.xy, shadowTexUV.z).r;
}

float GetShadowFactor_Direct(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float w = 2048.f;
    float sampleBiasSize = 1.f / w;
    
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    return ShadowMap.Sample(Point_Sampler, shadowTexUV.xy).r;
    
    //return ShadowMap.SampleCmpLevelZero(Shadow_Sampler, shadowTexUV.xy, shadowTexUV.z);
}

float GetShadowFactor_PCF_Sample4(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    float w = 2048.f;
    float sampleBiasSize = 1.f / w;
    
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    float s1 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy).r;
    float s2 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy + float2(sampleBiasSize, 0)).r;
    float s3 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy + float2(0, sampleBiasSize)).r;
    float s4 = ShadowMap.Sample(Point_Sampler, shadowTexUV.xy + float2(sampleBiasSize, sampleBiasSize)).r;
    
    shadowTexUV.xyz /= shadowTexUV.w;
    
    float shadowDepth = shadowTexUV.z;
    float result1 = shadowDepth <= s1;
    float result2 = shadowDepth <= s2;
    float result3 = shadowDepth <= s3;
    float result4 = shadowDepth <= s4;
    
    float2 uv = frac(w * shadowTexUV.xy);
    
    return lerp(
        lerp(result1, result2, uv.x),
        lerp(result3, result4, uv.x), 
        uv.y
    ); 
}

float GetShadowFactor_PCF_Sample9(float4 inWorldPosition, float4x4 inShadowMatrix)
{
    uint width = 0;
    uint height = 0;
    uint numMips = 0;
    
    ShadowMap.GetDimensions(0, width, height, numMips);
    float2 sampleBiasSize = float2(1.f / width, 1.f / height);
    
    const float2 constShadowSampleBias[9] = {
        float2(-sampleBiasSize.x, -sampleBiasSize.y),   float2(0, -sampleBiasSize.y),   float2(sampleBiasSize.x, -sampleBiasSize.y),
        float2(-sampleBiasSize.x, 0),                   float2(0, 0),                   float2(sampleBiasSize.x, 0),
        float2(-sampleBiasSize.x, sampleBiasSize.y),    float2(0, sampleBiasSize.y),    float2(sampleBiasSize.x, sampleBiasSize.y)
    };
    
    float4 shadowTexUV = mul(inShadowMatrix, inWorldPosition);
    shadowTexUV.xyz /= shadowTexUV.w;
    float result = 0.f;
    
    for (int i = 0; i < 9; i++)
    {
        result += ShadowMap.SampleCmpLevelZero(
            Shadow_Sampler,
            shadowTexUV.xy + constShadowSampleBias[i],
            shadowTexUV.z
        );
    }
    
    return max(result / 9.0f, 0.1f); 
}

float ProcessingImnidirectionalSampleCubeMapShadow(float4 inWorldPosition, float3 inLightPosition)
{
    float3 lightView = inWorldPosition.xyz - inLightPosition;
    
    float cubemapDepth = ShadowCubeMap[0].Sample(Point_Sampler, lightView).r;
    
    return cubemapDepth;
}

float ProcessingImnidirectionalSampleCmpLevelZeroShadow(float4 inWorldPosition, float3 inLightPosition)
{
    float3 lightView = inWorldPosition.xyz - inLightPosition;
    
    float cubemapDepth = ShadowCubeMap[0].SampleCmpLevelZero(Shadow_Sampler, float4(lightView, 1.f), 0.f).r;
    
    return cubemapDepth;
}

bool IsAngleRange(float inAngle, float x, float y)
{
    return inAngle >= min(x, y) && inAngle <= max(x, y);
}

bool IsAngleAxisRange(float inAngle, float x, float y, float inCriticalValue, bool bCompMin)
{
    if (bCompMin)
    {
        if (inAngle > inCriticalValue)
        {
            if (inAngle <= max(x, y))
            {
                return true;
            }
            
        }
        else if (inAngle < inCriticalValue)
        {
            if (inAngle >= min(x, y))
            {
                return true;
            }
        }
        else
                return true;
    }
    else
    {
        if (inAngle > 0.f)
        {
            if (inAngle >= max(x, y) && inAngle <= inCriticalValue)
            {
                return true;
            }
        }
        else
        {
            if (inAngle <= min(x, y) && inAngle >= -inCriticalValue)
            {
                return true;
            }
        }
    }
    return false;
}

bool IsPointInCubeMapViewport(
    float inPointTheta
    , float inPointPhi
    , float4 inCompareValue
    , int bAxisRange
    , float inCriticalValue
    , bool bCompMin
)
{
    if (IsAngleRange(inPointTheta, inCompareValue.x, inCompareValue.y))
    {
        if (bAxisRange == 0)
        {
            if (IsAngleAxisRange(
                inPointPhi
                , inCompareValue.z
                , inCompareValue.w
                , inCriticalValue
                , bCompMin
            ))
            {
                return true;
            }
        }
        else if (bAxisRange == 1)
        {
            if (inPointPhi >= inCompareValue.z && inPointPhi <= inCompareValue.w)
            {
                return true;
            }
        }
        else if (bAxisRange == 2)
        {
            if (inPointPhi <= inCompareValue.z && inPointPhi >= inCompareValue.w)
                return true;
        }
        else
        {
            if (IsAngleRange(inPointPhi, inCompareValue.z, inCompareValue.w))
            {
                return true;
            }
        }
    }
    return false;
}

float3 GetPointSphericalCoordinates(const float3 inV3d)
{
    float radius = dot(inV3d, inV3d);
    radius = sqrt(radius);
    float theta = acos(inV3d.y / radius);
    float phi = atan2(inV3d.z, inV3d.x);
    return float3(radius, theta, phi);
}

struct FCubemapAxialRangeR
{
    float4 PositiveX;
    float4 NegativeX;
    
    float4 PositiveY;
    float4 NegativeY;
    
    float4 PositiveZ;
    float4 NegativeZ;
};

int GetSampleCubemapIndex(float3 inPointPosition)
{
    
    FCubemapAxialRangeR cubemapAxialRangeR;
    cubemapAxialRangeR.PositiveX = float4(PI_DIV4, PI_DIV4 * 3, PI_DIV4, -PI_DIV4);
    cubemapAxialRangeR.NegativeX = float4(PI_DIV4, PI_DIV4 * 3, PI_DIV4 * 3, -PI_DIV4 * 3);
    cubemapAxialRangeR.PositiveY = float4(0, PI_DIV4, PI * 2, -PI * 2);
    cubemapAxialRangeR.NegativeY = float4(PI_DIV4 * 3, PI, PI * 2, -PI * 2);
    cubemapAxialRangeR.PositiveZ = float4(PI_DIV4, PI_DIV4 * 3, PI_DIV4, PI_DIV4 * 3);
    cubemapAxialRangeR.NegativeZ = float4(PI_DIV4, PI_DIV4 * 3, -PI_DIV4, -PI_DIV4 * 3);
    
    float3 sphericalCoords = GetPointSphericalCoordinates(inPointPosition);
    float theta = sphericalCoords.y;
    float phi = sphericalCoords.z;

    if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.PositiveX,3,0.f,true))
    {
        return 0; // ECubeMapFace::POSITIVE_X
    }
    else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.NegativeX, 0, PI,false))
    {
        return 1; // ECubeMapFace::NEGATIVE_X
    }
    else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.PositiveY, 3, 0, false))
    {
        return 2; // ECubeMapFace::POSITIVE_Y
    }
    else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.NegativeY, 3, 0, false))
    {
        return 3; // ECubeMapFace::NEGATIVE_Y
    }
    else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.PositiveZ, 1, 0, false))
    {
        return 4; // ECubeMapFace::POSITIVE_Z
    }
    else if (IsPointInCubeMapViewport(theta, phi, cubemapAxialRangeR.NegativeZ, 2, 0, false))
    {
        return 5; // ECubeMapFace::NEGATIVE_Z
    }
    
    
    return -1; // Invalid
}

float4 DebugCubeMapViewPort(int index, float max)
{
    float4 color = float4(0.f, 0.f, 0.f, 1.f);
    float positiveValue = 1.f;
    float negativeValue = .1f;
    if (index == 0)
    {
        color = float4(positiveValue, 0.f, 0.f, 1.f);
    }
    else if (index == 1)
    {
        color = float4(negativeValue, 0.f, 0.f, 1.f);
    }
    else if (index == 2)
    {
        color = float4(0.f, positiveValue, 0.f, 1.f);
    }
    else if (index == 3)
    {
        color = float4(0.f, negativeValue, 0.f, 1.f);
    }
    else if (index == 4)
    {
        color = float4(0.f, 0.f, positiveValue, 1.f);
    }
    else if (index == 5)
    {
        color = float4(0.f, 0.f, negativeValue, 1.f);
    }
    return color;
}
#endif
