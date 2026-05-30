#pragma once
#include "../../Mesh/Core/MeshManager.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/Geometry/RenderingData.h"

namespace MeshConstruction
{
    template<class T, typename ...ParamTypes>
    T* CreateMeshComponent(CMeshManager* inMeshMananger, T* inMeshComp, ParamTypes &&...params)
    {
        if (inMeshMananger && inMeshComp)
        {
            size_t meshHashKey = 0;
            inMeshComp->BuildKey(meshHashKey, forward<ParamTypes>(params)...);
            FRenderingData renderingData;
            if (inMeshMananger->GetRenderingPipeline().FindMeshRenderingData(meshHashKey, renderingData, (int)inMeshComp->GetRenderLayerType()))
            {
                inMeshMananger->GetRenderingPipeline().DuplicateMesh(inMeshComp, renderingData);
            }
            else
            {

                FMeshRenderingData meshRenderingData;
                inMeshComp->CreateMesh(meshRenderingData, forward<ParamTypes>(params)...);

                inMeshMananger->GetRenderingPipeline().BuildMesh(meshHashKey, inMeshComp, meshRenderingData);

                inMeshComp->Init();
            }
            return inMeshComp;  
        }
        return NULL;
    }
    
    template<class T, typename ...ParamTypes>
    T* CreateMeshComponent(CMeshManager* inMananger, ParamTypes &&...params)
    {
        if (inMananger)
        {
            T* myMeshComp = CreateObject<T>(new T());
            
            return CreateMeshComponent<T>(inMananger, myMeshComp, params...);
        }
        return NULL;
    }
}
