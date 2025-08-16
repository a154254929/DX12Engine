#pragma once
#include "Core/Mesh.h"

class CBoxMesh : public CMesh
{
	typedef CMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static CBoxMesh* CreateMesh(float inHeight, float inWidth, float inDepth);
};