#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "GeometryMap.h"

class CMesh;
class FRenderingPipeline: public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* inMesh, const FMeshRenderingData* inRenderingData);
protected:
	FGeometryMap geometryMap;
};