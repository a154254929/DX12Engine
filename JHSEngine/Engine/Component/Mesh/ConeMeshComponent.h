#pragma once
#include "Core/ShellMeshComponent.h"

class CConeMeshComponent : public CShellMeshComponent
{
public:
    CConeMeshComponent();

    void CreateMesh(
        FMeshRenderingData& meshRenderingData,
        float inRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );

    void BuildKey(
        size_t& meshHashKey,
        float inRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );
};