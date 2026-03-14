#pragma once
#include "../../TransformationComponent.h"

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
	
public:
	UINT GetMaterialNum() const;
	
	vector<CMaterial*>* GetMaterials() { return &materials; };
};