#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
    : bDirty(true)
    , baseColor(.5f, .5f, .5f, 1.f)
    , roughness(.2f)
    , materialType(EMaterialType::Lambertain)
    , materialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
    , materialTransform(EngineMath::IdentityMatrix4x4())
{
}

void CMaterial::SetBaseColor(fvector_4d inBaseColor)
{
    baseColor = inBaseColor;
    
    SetDirty(true);
}

void CMaterial::SetRoughness(float inRoughness)
{
    roughness = inRoughness; 
    
    SetDirty(true);
}

void CMaterial::SetMaterialType(EMaterialType inMaterialType)
{
    materialType = inMaterialType;
    
    SetDirty(true);
}

void CMaterial::SetBaseColorIndexKey(std::string& inAssetFileName)
{
    baseColorIndexKey = inAssetFileName;
    
    SetDirty(true);
}

void CMaterial::SetDirty(bool inDirty)
{
    bDirty = inDirty;
}
