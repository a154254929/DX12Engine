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

bool CMeshComponent::IsDynamicReflection() const
{
    if (materials.size() > 0)
    {
        return materials[0]->IsDynamicReflection() && meshRenderLayerType == RENDERLAYER_OPAQUE_REFLECTOR;
    }
    return false;
}
