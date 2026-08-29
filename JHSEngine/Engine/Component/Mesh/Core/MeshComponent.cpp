#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"

CMeshComponent::CMeshComponent()
{
    FCreateObjectParam param;
    param.owner = this;
    materials.push_back(CreateObject<CMaterial>(param, new CMaterial()));
    meshRenderLayerType = RENDERLAYER_OPAQUE;
    
    bCastShadow = true;
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
