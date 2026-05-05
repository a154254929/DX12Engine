#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
    : baseColor(.5f, .5f, .5f, 1.f)
    , roughness(.2f)
    , materialType(EMaterialType::Lambertain)
    , materialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
    , materialTransform(EngineMath::IdentityMatrix4x4())
{
}
