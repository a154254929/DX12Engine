#pragma once
#include "Core/Mesh.h"

class GSphereMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GSphereMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);
};