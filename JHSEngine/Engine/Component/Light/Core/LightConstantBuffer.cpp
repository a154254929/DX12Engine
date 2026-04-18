#include "LightConstantBuffer.h"
#include "LightType.h"

FLightConstantBuffer::FLightConstantBuffer()
    :lightInfo(0, 0, 0, 0)
{
    memset(&sceneLights, 0, sizeof(sceneLights));
}

FLight::FLight()
    : startAttenuation(1.0f)
    , endAttenuation(10.0f)
    , lightType((int)ELightType::None)
{
    
}