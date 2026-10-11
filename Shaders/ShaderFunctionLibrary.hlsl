float3x3 GetTBNMatrix(float3 InUnitWorldNormal, float3 InWorldTangent)
{
    float3 N = InUnitWorldNormal;
    float3 T = normalize(InWorldTangent - dot(N, InWorldTangent) * N);
    float3 B = cross(N, T);
    return float3x3(T, B, N);
}

float2 GetPolarCoordinate(float2 inTexcoord)
{
    float pi = 3.1415926535897932384626433832795f;
    float pi2 = pi * 2.f;
    
    float2 centerRelative = inTexcoord - .5f;
    float angle = atan2(centerRelative.y, centerRelative.x);
    float angleNormalized = (angle + pi) / pi2;
    float radius = length(centerRelative);
    
    float2 cdTexcoord = float2(angleNormalized, radius);
    
    return cdTexcoord;
}