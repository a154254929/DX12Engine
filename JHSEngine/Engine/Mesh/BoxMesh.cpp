#include "BoxMesh.h"
#include "Core/MeshType.h"

void GBoxMesh::Init()
{
	Super::Init();
}

void GBoxMesh::Draw(float deltaTime)
{
	Super::Draw(deltaTime);
}

void GBoxMesh::BuildMesh(const FMeshRenderingData* inRenderingData)
{
	Super::BuildMesh(inRenderingData);
}

GBoxMesh* GBoxMesh::CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, float inDepth)
{
	float halfHeight = inHeight * .5f;
	float halfWidth = inWidth * .5f;
	float halfDepth = inDepth * .5f;

	//构建顶点
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT4(Colors::White)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT4(Colors::AliceBlue)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT4(Colors::Aqua)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT4(Colors::Aquamarine)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT4(Colors::Bisque)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT4(Colors::Blue)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT4(Colors::Chocolate)));
	meshRenderingData.vertexData.push_back(FVertex(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT4(Colors::Red)));

	//构建索引
	//前
	meshRenderingData.indexData.push_back(0);meshRenderingData.indexData.push_back(1);meshRenderingData.indexData.push_back(2);
	meshRenderingData.indexData.push_back(0);meshRenderingData.indexData.push_back(2);meshRenderingData.indexData.push_back(3);
	//后
	meshRenderingData.indexData.push_back(4);meshRenderingData.indexData.push_back(7);meshRenderingData.indexData.push_back(6);
	meshRenderingData.indexData.push_back(4);meshRenderingData.indexData.push_back(6);meshRenderingData.indexData.push_back(5);
	//左
	meshRenderingData.indexData.push_back(4);meshRenderingData.indexData.push_back(5);meshRenderingData.indexData.push_back(1);
	meshRenderingData.indexData.push_back(4);meshRenderingData.indexData.push_back(1);meshRenderingData.indexData.push_back(0);
	//右
	meshRenderingData.indexData.push_back(3);meshRenderingData.indexData.push_back(2);meshRenderingData.indexData.push_back(6);
	meshRenderingData.indexData.push_back(3);meshRenderingData.indexData.push_back(6);meshRenderingData.indexData.push_back(7);
	//上
	meshRenderingData.indexData.push_back(1);meshRenderingData.indexData.push_back(5);meshRenderingData.indexData.push_back(6);
	meshRenderingData.indexData.push_back(1);meshRenderingData.indexData.push_back(6);meshRenderingData.indexData.push_back(2);
	//下
	meshRenderingData.indexData.push_back(4);meshRenderingData.indexData.push_back(0);meshRenderingData.indexData.push_back(3);
	meshRenderingData.indexData.push_back(4);meshRenderingData.indexData.push_back(3);meshRenderingData.indexData.push_back(7);

	GBoxMesh* boxMesh = new GBoxMesh;
	boxMesh->BuildMesh(&meshRenderingData);

	boxMesh->Init();

	return boxMesh;
}