#pragma once
#include "../../EngineMinimal.h"

class FShader
{
public:
	LPVOID GetBufferPointer();
	SIZE_T GetBufferSize();
	
	void BuildShaders(const wstring& inFileName, const string& inEntryFunName, const string& inShaderVersion);

private:
	ComPtr<ID3DBlob> shaderCode;
};