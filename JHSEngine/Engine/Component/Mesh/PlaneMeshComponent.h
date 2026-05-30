#pragma once
#include "Core/ShellMeshComponent.h""

class CPlaneMeshComponent : public CShellMeshComponent
{
public:
    CPlaneMeshComponent();
    
    void CreateMesh(
        FMeshRenderingData& meshRenderingData,
        float inHeight,
        float inWidth,
        uint32_t inHeightSubdivide,
        uint32_t inWidthSubdivide
    );
    
    void BuildKey(
        size_t& meshHashKey,
        float inHeight,
        float inWidth,
        uint32_t inHeightSubdivide,
        uint32_t inWidthSubdivide
    );
};