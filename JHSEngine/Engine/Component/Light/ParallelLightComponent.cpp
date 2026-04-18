#include "ParallelLightComponent.h"
#include "../../Mesh/Core/MeshManager.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../Mesh/Core/MeshComponent.h"

CParallelLightComponent::CParallelLightComponent()
{
    //读取模型资源
    static string meshPath = "../JHSEngine/Asset/SunMesh.obj";
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
}
