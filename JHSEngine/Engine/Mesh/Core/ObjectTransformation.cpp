#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
    : world(EngineMath::IdentityMatrix4x4())
    , textureTransformation(EngineMath::IdentityMatrix4x4())
{
}