#pragma once
#include "Core/Mesh.h"

class GCylinderMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GCylinderMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inTopRadius, float inBottomRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);
};