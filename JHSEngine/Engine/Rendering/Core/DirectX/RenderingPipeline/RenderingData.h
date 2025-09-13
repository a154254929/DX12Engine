#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
//#include "../../RenderingResourceUpdate.h"

class CMesh;
struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	FRenderingData();

	UINT indexSize;//index数量
	UINT vertexSize;//顶点数量

	UINT indexOffsetPosition;
	UINT vertexOffsetPosition;

	UINT indexTypeSize;
	UINT vertexTypeSize;

public:
	UINT GetVertexSizeInBytes() const { return vertexSize * vertexTypeSize; }
	UINT GetIndexSizeInBytes() const { return indexSize * indexTypeSize; }
public:
	DXGI_FORMAT indexFormat;

	XMFLOAT4X4 worldMatrix;
	CMesh* mesh;
	//shared_ptr<FRenderingResourcesUpdate> objectContants;
};