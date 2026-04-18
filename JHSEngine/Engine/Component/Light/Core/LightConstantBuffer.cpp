#include "LightConstantBuffer.h"

FLightConstantBuffer::FLightConstantBuffer()
    :lightInfo(0, 0, 0, 0)
{
    memset(&screenLight, 0, sizeof(screenLight));
}
