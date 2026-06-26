#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
    : bDirty(true)
    , materialIndex(-1)
    , baseColor(.5f, .5f, .5f, 1.f)
    , specularColor(.5f, .5f, .5f)
    , roughness(.2f)
    , materialType(EMaterialType::Lambertain)
    , materialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
    , materialTransform(EngineMath::IdentityMatrix4x4())
{
}

void CMaterial::SetDirty(bool inDirty)
{
    bDirty = inDirty;
}

void CMaterial::SetMaterialIndex(int inMaterialIndex)
{
    materialIndex = inMaterialIndex;
    
    SetDirty(true);
}

void CMaterial::SetBaseColor(const fvector_4d inBaseColor)
{
    baseColor = inBaseColor;
    
    SetDirty(true);
}

void CMaterial::SetSpecularColor(const fvector_3d& inSpecularColor)
{
    specularColor = inSpecularColor;
    
    SetDirty(true);
}

void CMaterial::SetRoughness(const float inRoughness)
{
    roughness = inRoughness; 
    
    SetDirty(true);
}

void CMaterial::SetMaterialType(EMaterialType inMaterialType)
{
    materialType = inMaterialType;
    
    SetDirty(true);
}

void CMaterial::SetFresnelF0(const fvector_3d& inFresnelF0)
{
    fresenlF0 = inFresnelF0;
    SetDirty(true);
}

void CMaterial::SetBaseColorIndexKey(const std::string inAssetFileName)
{
    baseColorIndexKey = inAssetFileName;
    
    SetDirty(true);
}

void CMaterial::SetNormalMapIndexKey(const std::string inAssetFileName)
{
    normalMapIndexKey = inAssetFileName;
    
    SetDirty(true);
}

void CMaterial::SetSpecularMapIndexKey(const std::string inAssetFileName)
{
    specularMapIndexKey = inAssetFileName;
    
    SetDirty(true);
}

void CMaterial::SetRoughnessMapIndexKey(const std::string inAssetFileName)
{
    roughnessMapIndexKey = inAssetFileName;
    
    SetDirty(true);
}
