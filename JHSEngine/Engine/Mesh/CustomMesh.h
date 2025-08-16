#pragma once
#include "Core/Mesh.h"

class CCustomMesh : public CMesh
{
	typedef CMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void Draw(float deltaTime);

	static CCustomMesh* CreateMesh(string &inPath);

	static bool LoadObjFromBuff(char* buff, uint32_t buffSize, FMeshRenderingData* meshData);
};