#include "ShaderType.h"

bool ShaderType::ToD3DShaderMacro(const vector<FShaderMacro>& inShaderMacro, vector<D3D_SHADER_MACRO>& outD3DMacro)
{
    for (size_t i = 0; i < inShaderMacro.size(); i++)
    {
        D3D_SHADER_MACRO shaderMacro = {
            inShaderMacro[i].Name.c_str(), inShaderMacro[i].Definition.c_str()
        };
        
        outD3DMacro.push_back(shaderMacro);
    }
    
    if (inShaderMacro.size() > 0)
    {
        D3D_SHADER_MACRO shaderMacro;
        shaderMacro.Name = NULL;
        shaderMacro.Definition = NULL;
        outD3DMacro.push_back(shaderMacro);
    }
    
    return outD3DMacro.size();
}
