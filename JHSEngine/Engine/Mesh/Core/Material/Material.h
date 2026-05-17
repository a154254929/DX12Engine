#pragma once
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
    CMaterial();

    void SetMaterialDisplayStatusType(EMaterialDisplayStatusType inDisplayType) { materialDisplayStatusType = inDisplayType; }
    EMaterialDisplayStatusType GetMaterialDisplayStatusType() { return materialDisplayStatusType; }
    void SetDirty(bool inDirty);
    void SetMaterialIndex(int inMaterialIndex);
    void SetBaseColor(fvector_4d inBaseColor);
    void SetSpecularColor(fvector_3d& inSpecularColor);
    void SetRoughness(float inRoughness);
    void SetMaterialType(EMaterialType inMaterialType);
    void SetBaseColorIndexKey(std::string inAssetFileName);
    void SetNormalMapIndexKey(std::string inAssetFileName);
    void SetSpecularMapIndexKey(std::string inAssetFileName);

    FORCEINLINE bool IsDirty() const { return true; }
    FORCEINLINE int GetMaterialIndex() const { return materialIndex; }
    FORCEINLINE fvector_4d GetBaseColor() { return baseColor; }
    FORCEINLINE const fvector_3d& GetSpecularColor() { return specularColor; }
    FORCEINLINE float GetRoughness() { return roughness; }
    FORCEINLINE EMaterialType GetMaterialType() { return materialType; }
    FORCEINLINE XMFLOAT4X4& GetMaterialTransform() { return materialTransform; }
    FORCEINLINE const string& GetBaseColorIndexKey() { return baseColorIndexKey; }
    FORCEINLINE const string& GetNormalMapIndexKey() { return normalMapIndexKey; }
    FORCEINLINE const string& GetSpecularMapIndexKey() { return specularMapIndexKey; }
private:
    bool bDirty;
    int materialIndex;
    
    fvector_4d baseColor;
    fvector_3d specularColor;
    UINT xx;
    
    float roughness;
    EMaterialType materialType;
    
    std::string baseColorIndexKey;
    std::string normalMapIndexKey;
    std::string specularMapIndexKey;
    
    EMaterialDisplayStatusType materialDisplayStatusType;
    XMFLOAT4X4 materialTransform;
};