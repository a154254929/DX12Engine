#pragma once
#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
	typedef CMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static CSphereMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);
};