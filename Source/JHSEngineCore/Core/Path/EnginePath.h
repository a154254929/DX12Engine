#pragma once

#include "JHSEngineCoreMacro.h"
#include "JHSEngineCore.h"

struct JHS_ENGINE_CORE_API FEnginePathHelper
{
    static std::string GetEngineRootPath();
    
    static std::string GetEngineRelativeLogsPath();
    
    static std::string GetEngineContentRelativeLogsPath();
    
    static std::wstring GetEngineShaderRelativeLogsPath();
    
    static std::string RelativeToAbsolutePath(const std::string& relativePath);
};
