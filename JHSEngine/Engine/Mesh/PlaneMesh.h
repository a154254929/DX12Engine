#pragma once
#include "Core/Mesh.h"

class CPlaneMesh : public CMesh
{
	typedef CMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static CPlaneMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide);
};