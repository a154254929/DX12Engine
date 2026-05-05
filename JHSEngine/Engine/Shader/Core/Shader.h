#pragma once
#include "../../EngineMinimal.h"

class FShader
{
public:
    LPVOID GetBufferPointer() const;
    SIZE_T GetBufferSize() const;
    
    void BuildShaders(
        const wstring& inFileName,
        const string& inEntryFunName,
        const string& inShaderVersion,
        const D3D_SHADER_MACRO* inShaderMacro = NULL
    );

private:
    ComPtr<ID3DBlob> shaderCode;
};