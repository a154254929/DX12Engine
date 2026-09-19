#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
    : world(EngineMath::IdentityMatrix4x4())
    , textureTransformation(EngineMath::IdentityMatrix4x4())
    , materialIndex(0)
    , RR0(0)
    , RR1(0)
    , RR2(0)
{
}