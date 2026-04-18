#include "LightConstantBuffer.h"
#include "LightType.h"

FLightConstantBuffer::FLightConstantBuffer()
    :lightInfo(0, 0, 0, 0)
{
    memset(&screenLight, 0, sizeof(screenLight));
}

FLight::FLight()
    : startAttenuation(1.0f)
    , endAttenuation(10.0f)
    , lightType((int)ELightType::None)
{
    
}