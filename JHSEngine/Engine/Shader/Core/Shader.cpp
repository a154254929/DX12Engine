#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
	return shaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
	return shaderCode->GetBufferSize();
}

void FShader::BuildShaders(const wstring& inFileName, const string& inEntryFunName, const string& inShaderVersion)
{
	ComPtr<ID3DBlob> errorShaderMsg;
	HRESULT compileResult = D3DCompileFromFile(
		inFileName.c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		inEntryFunName.c_str(),
		inShaderVersion.c_str(),
#ifdef _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		0,
#endif
		0,
		&shaderCode,
		&errorShaderMsg
	);
	if (errorShaderMsg)
	{
		Engine_Log_Error("%s", (char*)errorShaderMsg->GetBufferPointer());
	}

	ANALYSIS_HRESULT(compileResult);
}
