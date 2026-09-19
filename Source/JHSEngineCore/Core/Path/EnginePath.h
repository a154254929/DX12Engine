#pragma once

#include "JHSEngineCoreMacro.h"
#include "JHSEngineCore.h"

struct JHS_ENGINE_CORE_API FEnginePathHelper
{
    static std::string GetEngineRootPath();
    
    static std::string GetEngineRelativeLogsPath();
    
    static std::string GetEngineRelativeContentPath();
    
    static std::wstring GetEngineRelativeShadersPath();
    
    static std::string GetEngineRelativeBinariesPath();
    
    static std::string RelativeToAbsolutePath(const std::string& relativePath);
};
