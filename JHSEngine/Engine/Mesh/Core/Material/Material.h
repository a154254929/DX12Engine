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
    void SetBaseColor(const fvector_4d inBaseColor);
    void SetSpecularColor(const fvector_3d& inSpecularColor);
    void SetRoughness(const float inRoughness);
    void SetMaterialType(EMaterialType inMaterialType);
    void SetFresnelF0(const fvector_3d& inFresnelF0);
    void SetBaseColorIndexKey(const std::string inAssetFileName);
    void SetNormalMapIndexKey(const std::string inAssetFileName);
    void SetSpecularMapIndexKey(const std::string inAssetFileName);
    void SetRoughnessMapIndexKey(const std::string inAssetFileName);
    void SetDynamicReflection(bool inDynamicReflection);

    FORCEINLINE bool IsDirty() const { return true; }
    FORCEINLINE int GetMaterialIndex() const { return materialIndex; }
    FORCEINLINE fvector_4d GetBaseColor() { return baseColor; }
    FORCEINLINE const fvector_3d& GetSpecularColor() { return specularColor; }
    FORCEINLINE float GetRoughness() { return roughness; }
    FORCEINLINE fvector_3d& GetFresenlF0() { return fresenlF0; }
    FORCEINLINE EMaterialType GetMaterialType() { return materialType; }
    FORCEINLINE XMFLOAT4X4& GetMaterialTransform() { return materialTransform; }
    FORCEINLINE const string& GetBaseColorIndexKey() { return baseColorIndexKey; }
    FORCEINLINE const string& GetNormalMapIndexKey() { return normalMapIndexKey; }
    FORCEINLINE const string& GetSpecularMapIndexKey() { return specularMapIndexKey; }
    FORCEINLINE const string& GetRoughnessMapIndexKey() { return roughnessMapIndexKey; }
    FORCEINLINE bool IsDynamicReflection() { return bDynamicReflection 
        && (
            materialType == BlinnPhong
            || materialType == Back
            || materialType == PBR
            || materialType == Phong
        ); 
    }
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
    std::string roughnessMapIndexKey;
    
    fvector_3d fresenlF0;
    
    EMaterialDisplayStatusType materialDisplayStatusType;
    XMFLOAT4X4 materialTransform;
    
    bool bDynamicReflection ;
};