#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Component/TransformationComponent.h"

CMesh::CMesh()
{
	transformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void CMesh::Init()
{
}

void CMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
}

void CMesh::PreDraw(float deltaTime)
{
}

void CMesh::Draw(float deltaTime)
{
}

void CMesh::PostDraw(float deltaTime)
{
}

void CMesh::SetPosition(const XMFLOAT3& inPosition)
{
	transformationComponent->SetPosition(inPosition);
}

void CMesh::SetRotation(const fvector_3d& inRotation)
{
}

XMFLOAT3& CMesh::GetPosition()
{
	return transformationComponent->GetPosition();
}

XMFLOAT3& CMesh::GetForwardVector()
{
	return transformationComponent->GetForwardVector();
}

XMFLOAT3& CMesh::GetRightVector()
{
	return transformationComponent->GetRightVector();
}

XMFLOAT3& CMesh::GetUpVector()
{
	return transformationComponent->GetUpVector();
}
