#pragma once
#include "Core/Mesh.h"

class CAesmaMesh : public CMesh
{
	typedef CMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static CAesmaMesh* CreateMesh(
		FMeshRenderingData& meshRenderingData,
		float inOuterRadius,
		float inInnerRadius,
		float inHeight,
		uint32_t inAxialSubdivision,
		uint32_t inHeightSubdivision
	);
};