#pragma once
#include "Core/ShellMeshComponent.h"

//模型读取类型
enum EMeshAssetType
{
    EMesh_OOJ,
    EMesh_FBX
};

class CCustomMeshComponent : public CShellMeshComponent
{
public:
    CCustomMeshComponent();

    void CreateMesh(FMeshRenderingData& meshRenderingData, const string& inPath);
    
    void BuildKey(size_t& meshHashKey, const string& inPath);

    static bool LoadObjFromBuff(char* buff, uint32_t buffSize, FMeshRenderingData* meshData);
    
    static bool LoadFbxFromBuff(const string& inPath, FMeshRenderingData& meshData);
};
