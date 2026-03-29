#pragma once
#include "ShellMeshComponent.h"

class CCustomMeshComponent : public CShellMeshComponent
{
public:
    CCustomMeshComponent();

    void CreateMesh(FMeshRenderingData& meshRenderingData, string &inPath);

    static bool LoadObjFromBuff(char* buff, uint32_t buffSize, FMeshRenderingData* meshData);
};