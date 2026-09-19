#pragma once
#include "Core/ShellMeshComponent.h"

class CDonutMeshComponent : public CShellMeshComponent
{
public:
    CDonutMeshComponent();

    void CreateMesh(
        FMeshRenderingData& meshRenderingData,
        float inDonutRadius,
        float inThicknessRadius,
        uint32_t inDonutAxialSubdivision,
        uint32_t inThicknessAxialSubdivision
    );

    void BuildKey(
        size_t& meshHashKey,
        float inDonutRadius,
        float inThicknessRadius,
        uint32_t inDonutAxialSubdivision,
        uint32_t inThicknessAxialSubdivision
    );
};