#include "MoveArrow.h"

#include "Engine/Core/Consttruction/MacroConstruction.h"

GMoveArrow::GMoveArrow()
{
    FCreateObjectParam param;
    param.owner = this;
    
    xAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    yAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
    zAxisComponent = ConstructionObject<CCustomMeshComponent>(param);
}

void GMoveArrow::CreateMesh()
{
    string meshPath = FEnginePathHelper::GetEngineRelativeContentPath() + "\\Handle\\MoveArrow.fbx";
    
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, xAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, yAxisComponent, meshPath);
    CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, zAxisComponent, meshPath);
    
    xAxisComponent->SetRotation(fvector_3d(0.f, 90.f, 0.f));
    yAxisComponent->SetRotation(fvector_3d(90.f, 0.f, 0.f));
}
