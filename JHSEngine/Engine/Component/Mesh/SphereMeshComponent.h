#pragma once
#include "ShellMeshComponent.h"

class CSphereMeshComponent : public CShellMeshComponent
{
public:
    CSphereMeshComponent();

    void CreateMesh(
        FMeshRenderingData& meshRenderingData,
        float inRadius,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision,
        bool bReverse = false);

    void BuildKey(
        size_t& meshHashKey,
        float inRadius,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision,
        bool bReverse = false);
};
