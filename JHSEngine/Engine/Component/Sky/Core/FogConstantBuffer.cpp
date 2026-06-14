#include "FogConstantBuffer.h"

FFogConstantBuffer::FFogConstantBuffer()
    : fogColor(1.0f, 1.0f, 1.0f)
    , fogStart(10.0f)
    , fogRange(100.0f)
    , fogHeight(100.0f)
    , fogTransparentCoefficient(0.0f)
{
}
