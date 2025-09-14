#pragma once
#include "Core/Mesh.h"

class GConeMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GConeMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);
};