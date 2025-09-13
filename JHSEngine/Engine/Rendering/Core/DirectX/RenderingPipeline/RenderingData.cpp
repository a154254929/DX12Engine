#include "RenderingData.h"
#include "../../../../Mesh/Core/MeshType.h"

FRenderingData::FRenderingData()
	:indexSize(0)
	, vertexSize(0)
	, indexOffsetPosition(0)
	, vertexOffsetPosition(0)
	, indexTypeSize(sizeof(uint16_t))
	, vertexTypeSize(sizeof(FVertex))
{
}
