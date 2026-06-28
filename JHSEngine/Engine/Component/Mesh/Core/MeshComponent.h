#pragma once
#include "../../TransformationComponent.h"
#include "MeshComponentType.h"

class CMaterial;
struct FMeshRenderingData;
class CMeshComponent : public CTransformationComponent
{
    CVARIABLE()
    vector<CMaterial*> materials;
    
public:
    CMeshComponent();
    
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* inRenderingData);
    
    void SetRenderLayerType(EMeshRenderLayerType inMeshRenderLayerType) {meshRenderLayerType = inMeshRenderLayerType;};
    
public:
    UINT GetMaterialNum() const;
    EMeshRenderLayerType GetRenderLayerType() const {return meshRenderLayerType;};
    
    vector<CMaterial*>* GetMaterials() { return &materials; };
    
    EMeshRenderLayerType meshRenderLayerType;
    
public:
    bool IsDynamicReflection() const;
};