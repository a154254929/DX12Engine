#pragma once
#include "ShellMeshComponent.h"

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
};