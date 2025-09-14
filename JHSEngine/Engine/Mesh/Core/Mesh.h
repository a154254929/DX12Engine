#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"

class CTransformationComponent;
class CMesh : public CCoreMinimalObject, public IRenderingInterface
{
	CVARIABLE()
	CTransformationComponent* transformationComponent;

public:
	CMesh();
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* inRenderingData);

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

public:
	void SetPosition(const XMFLOAT3& inPosition);
	void SetRotation(const fvector_3d& inRotation);

	XMFLOAT3& GetPosition();
	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();

};