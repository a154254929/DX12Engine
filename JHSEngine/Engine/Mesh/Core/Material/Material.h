#pragma once
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
    CMaterial();

    void SetMaterialDisplayStatusType(EMaterialDisplayStatusType inDisplayType) { materialDisplayStatusType = inDisplayType; }
    EMaterialDisplayStatusType GetMaterialDisplayStatusType() { return materialDisplayStatusType; }
    void SetBaseColor(fvector_4d inBaseColor) { baseColor = inBaseColor; }
    void SetRoughness(float inRoughness) { roughness = inRoughness; }
    void SetMaterialType(EMaterialType inMaterialType) { materialType = inMaterialType; }

    FORCEINLINE fvector_4d GetBaseColor() { return baseColor; }
    FORCEINLINE float GetRoughness() { return roughness; }
    FORCEINLINE EMaterialType GetMaterialType() { return materialType; }
private:
    fvector_4d baseColor;
    float roughness;
    EMaterialType materialType;
    
    EMaterialDisplayStatusType materialDisplayStatusType;
};