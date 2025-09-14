#pragma once
#include "Core/Mesh.h"

class GDonutMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GDonutMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision);
};