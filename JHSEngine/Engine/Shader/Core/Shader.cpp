#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
    return shaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
    return shaderCode->GetBufferSize();
}

void FShader::BuildShaders(
    const wstring& inFileName,
    const string& inEntryFunName,
    const string& inShaderVersion,
    const D3D_SHADER_MACRO* inShaderMacro
)
{
    ComPtr<ID3DBlob> errorShaderMsg;
    HRESULT compileResult = D3DCompileFromFile(
        inFileName.c_str(),
        inShaderMacro,
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
    if (FAILED(compileResult))
    {
        char errorBuf[2048];
        if (errorShaderMsg)
        {
            const char* shaderError = (char*)errorShaderMsg->GetBufferPointer();
            sprintf_s(errorBuf, 2048, "Shader Compile Failed:\nFile: %ws\nEntry: %s\nVersion: %s\n\n%s", 
                inFileName.c_str(), 
                inEntryFunName.c_str(), 
                inShaderVersion.c_str(),
                shaderError);
            Engine_Log_Error("%s", errorBuf);
            MessageBoxA(NULL, errorBuf, "Shader Compile Error", MB_OK | MB_ICONERROR);
        }
        else
        {
            sprintf_s(errorBuf, 2048, "Shader Compile Failed:\nFile: %ws\nEntry: %s\nVersion: %s\n\nNo error message available. HRESULT: 0x%08X", 
                inFileName.c_str(), 
                inEntryFunName.c_str(), 
                inShaderVersion.c_str(),
                compileResult);
            Engine_Log_Error("%s", errorBuf);
            MessageBoxA(NULL, errorBuf, "Shader Compile Error", MB_OK | MB_ICONERROR);
        }
        assert(0);
    }
}
