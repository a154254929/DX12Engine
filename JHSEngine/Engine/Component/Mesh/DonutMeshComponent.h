#pragma once
#include "ShellMeshComponent.h"

class CDonutMeshComponent : public CShellMeshComponent
{
public:
    CDonutMeshComponent();

    void CreateMesh(FMeshRenderingData& meshRenderingData, float inDonutRadius, float inThicknessRadius, uint32_t inDonutAxialSubdivision, uint32_t inThicknessAxialSubdivision);
};