#pragma once
#include "Core/ShellMeshComponent.h"

class CAesmaMeshComponent : public CShellMeshComponent
{
public:
    CAesmaMeshComponent();

    void CreateMesh(
        FMeshRenderingData& meshRenderingData,
        float inOuterRadius,
        float inInnerRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );

    void BuildKey(
        size_t& meshHashKey,
        float inOuterRadius,
        float inInnerRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );
};