#pragma once
#include "MeshConstruction.h"
#include "../../Mesh/Core/Mesh.h"

class GMesh;

namespace ActorMeshConstruction
{
    template<class T, typename ...ParamTypes>
    T* CreateMeshRenderData(CMeshManager* inMananger, GMesh* inGMesh, ParamTypes &&...params)
    {
        return MeshConstruction::CreateMeshComponent(inMananger, inGMesh->GetMeshComponent<T>(), std::forward<ParamTypes>(params)...);
    }
}
