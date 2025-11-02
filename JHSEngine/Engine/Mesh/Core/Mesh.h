#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "../../Actor/Core/ActorObject.h"

class CMaterial;
class GMesh : public GActorObject, public IRenderingInterface
{

	CVARIABLE()
		vector<CMaterial*> materials;

public:
	GMesh();
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

public:
	UINT GetMaterialNum() const;

	vector<CMaterial*>* GetMaterials() { return &materials; };

};