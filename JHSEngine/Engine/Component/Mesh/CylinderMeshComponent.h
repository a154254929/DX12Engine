#pragma once
#include "ShellMeshComponent.h"
class CCylinderMeshComponent : public CShellMeshComponent
{
public:
    CCylinderMeshComponent();

    void CreateMesh(
        FMeshRenderingData& meshRenderingData,
        float inTopRadius,
        float inBottomRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );

    void BuildKey(
        size_t& meshHashKey,
        float inTopRadius,
        float inBottomRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );
};