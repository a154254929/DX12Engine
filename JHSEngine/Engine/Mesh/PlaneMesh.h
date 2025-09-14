#pragma once
#include "Core/Mesh.h"

class GPlaneMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GPlaneMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, uint32_t inHeightSubdivide, uint32_t inWidthSubdivide);
};