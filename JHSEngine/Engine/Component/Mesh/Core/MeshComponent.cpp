#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"

CMeshComponent::CMeshComponent()
{
    materials.push_back(CreateObject<CMaterial>(new CMaterial()));
    meshRenderLayerType = RENDERLAYER_OPAQUE;
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* inRenderingData)
{
}

UINT CMeshComponent::GetMaterialNum() const
{
    return materials.size();
}