#include "SpotLightComponent.h"
#include "../../Mesh/Core/MeshManager.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../Mesh/Core/MeshComponent.h"

CSpotLightComponent::CSpotLightComponent()
    : Super()
{
    //读取模型资源
    static string meshPath = "../JHSEngine/Asset/SpotLightMesh.obj";
    SetLightMesh(GetMeshManager()->CreateMeshComponent(meshPath));
    
    //设置现况模式
    if (GetLightMesh())
    {
        if (CMaterial* sunMaterial = (*GetLightMesh()->GetMaterials())[0])
        {
            sunMaterial->SetMaterialType(EMaterialType::BaseColor);
            sunMaterial->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
            sunMaterial->SetBaseColor(fvector_4d(1.f, .7f, 1.f, 1.f));
        }
    }
    
    lightType = ELightType::SpotLight;
}

void CSpotLightComponent::SetConicalInnerCorner(float inConicalInnerCorner)
{
    conicalInnerCorner = inConicalInnerCorner;
    conicalOuterCorner = max(inConicalInnerCorner, conicalOuterCorner);
}

void CSpotLightComponent::SetConicalOuterCorner(float inConicalOuterCorner)
{
    conicalOuterCorner = inConicalOuterCorner;
    conicalInnerCorner = min(inConicalOuterCorner, conicalOuterCorner);
}
