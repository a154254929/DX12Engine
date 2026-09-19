#include "Sky.h"
#include "../../Component/Mesh/Core/MeshComponentType.h"
#include "../../Mesh/Core/Material/Material.h"

GSky::GSky()
    : Super()
{
    SetRenderLayerType(EMeshRenderLayerType::RENDERLAYER_BACKGROUND);
    CreateMesh(1, 20, 20, true);
            
    SetScale(fvector_3d(999.f, 999.f, 500.f));
    if (CMaterial* material = (*GetMaterials())[0])
    {
        //material->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));
        material->SetBaseColor(fvector_4d(0.f, 0.f, 0.f, 1.f));
        material->SetBaseColorIndexKey("../JHSEngine/Asset/Texture/EpicQuadPanorama_CC.dds");
        material->SetMaterialType(EMaterialType::BaseColor);
    }
    
}

void GSky::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}
