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
    void SetRoughness(float inRoughness);
    void SetMaterialType(EMaterialType inMaterialType);
    void SetBaseColorIndexKey(std::string &inAssetFileName);

    FORCEINLINE bool IsDirty() const { return true; }
    FORCEINLINE int GetMaterialIndex() const { return materialIndex; }
    FORCEINLINE fvector_4d GetBaseColor() { return baseColor; }
    FORCEINLINE float GetRoughness() { return roughness; }
    FORCEINLINE EMaterialType GetMaterialType() { return materialType; }
    FORCEINLINE XMFLOAT4X4& GetMaterialTransform() { return materialTransform; }
    FORCEINLINE const string& GetBaseColorIndexKey() { return baseColorIndexKey; }
private:
    bool bDirty;
    int materialIndex;
    
    fvector_4d baseColor;
    float roughness;
    EMaterialType materialType;
    
    std::string baseColorIndexKey;
    
    EMaterialDisplayStatusType materialDisplayStatusType;
    XMFLOAT4X4 materialTransform;
};