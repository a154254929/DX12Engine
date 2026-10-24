#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"
#include "Engine/Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

CMeshComponent::CMeshComponent()
{
    FCreateObjectParam param;
    param.owner = this;
    materials.push_back(CreateObject<CMaterial>(param, new CMaterial()));
    meshRenderLayerType = RENDERLAYER_OPAQUE;
    
    bCastShadow = true;
    bVisible = true;
    bPickup = true;
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* inRenderingData)
{
}

BoundingBox CMeshComponent::GetBoundingBox()
{
    BoundingBox aabb;
    GetBoundingBox(aabb);
    return aabb;
}

void CMeshComponent::GetBoundingBox(BoundingBox& outBoundingBox)
{
    FGeometry::FindRenderingDatas(
        [&](std::shared_ptr<FRenderingData>& inRenderingData)->EFindValueType
        {
            if (inRenderingData->meshComp == this)
            {
                outBoundingBox = inRenderingData->boundingBox;
                return EFindValueType::EFindValueType_Complete;
            }
            return EFindValueType::EFindValueType_In_progress;
        }
    );
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
