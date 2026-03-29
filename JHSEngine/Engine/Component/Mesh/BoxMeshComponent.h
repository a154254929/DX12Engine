#pragma once
#include "ShellMeshComponent.h"

class CBoxMeshComponent : public CShellMeshComponent
{
public:
    CBoxMeshComponent();

    void CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, float inDepth);
};