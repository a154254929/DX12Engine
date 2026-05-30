#pragma once
#include "Core/ShellMeshComponent.h"

class CBoxMeshComponent : public CShellMeshComponent
{
public:
    CBoxMeshComponent();

    void CreateMesh(FMeshRenderingData& meshRenderingData, float inHeight, float inWidth, float inDepth);
    
    void BuildKey(size_t& meshHashKey, float inHeight, float inWidth, float inDepth);
};