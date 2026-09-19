#include "EnginePath.h"

std::string FEnginePathHelper::GetEngineRootPath()
{
    return "";
}

std::string FEnginePathHelper::GetEngineRelativeLogsPath()
{
    return "..\\..\\Saved/Logs";
}

std::string FEnginePathHelper::GetEngineContentRelativeLogsPath()
{
    return "..\\..\\Content";
}

std::wstring FEnginePathHelper::GetEngineShaderRelativeLogsPath()
{
    return L"..\\..\\Shaders";
}

std::string FEnginePathHelper::RelativeToAbsolutePath(const std::string& relativePath)
{
    char pathBuff[1024] = { 0 };
    get_full_path(pathBuff, 1024, relativePath.c_str());
    
    return pathBuff;
}
