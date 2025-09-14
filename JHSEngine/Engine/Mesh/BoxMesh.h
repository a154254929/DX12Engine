#pragma once
#include "Core/Mesh.h"

class GBoxMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GBoxMesh* CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, float inDepth);
};