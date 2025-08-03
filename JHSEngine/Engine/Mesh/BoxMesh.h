#pragma once
#include "Core/Mesh.h"

class FBoxMesh : public FMesh
{
	typedef FMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static FBoxMesh* CreateMesh();
};