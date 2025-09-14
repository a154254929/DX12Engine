#pragma once
#include "Core/Mesh.h"

class GAesmaMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GAesmaMesh* CreateMesh(
		FMeshRenderingData& meshRenderingData,
		float inOuterRadius,
		float inInnerRadius,
		float inHeight,
		uint32_t inAxialSubdivision,
		uint32_t inHeightSubdivision
	);
};