#pragma once
#include "Core/Mesh.h"

class GCustomMesh : public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static GCustomMesh* CreateMesh(FMeshRenderingData& meshRenderingData, string &inPath);

	static bool LoadObjFromBuff(char* buff, uint32_t buffSize, FMeshRenderingData* meshData);
};