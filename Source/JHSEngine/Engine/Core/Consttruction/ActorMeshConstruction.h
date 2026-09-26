#pragma once
#include "MeshConstruction.h"
#include "../../Mesh/Core/Mesh.h"

class GMesh;

namespace ActorMeshConstruction
{
    template<class T, typename ...ParamTypes>
    T* CreateMeshRenderData(CMeshManager* inMananger, GMesh* inGMesh, ParamTypes &&...params)
    {
        return MeshConstruction::CreateMeshComponent<T>(inMananger, inGMesh->GetMeshComponent<T>(), std::forward<ParamTypes>(params)...);
    }
    
    template<class T, typename ...ParamTypes>
    T* CreateMeshRenderDataByComponent(CMeshManager* inMananger, T* inMeshComponent, ParamTypes &&...params)
    {
        return MeshConstruction::CreateMeshComponent<T>(inMananger, inMeshComponent, std::forward<ParamTypes>(params)...);
    }
}
