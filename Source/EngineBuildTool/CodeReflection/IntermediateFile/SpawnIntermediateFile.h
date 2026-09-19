#pragma once

#include "simple_library/public/simple_library.h"
#include "../CppSourceCodeType.h"

namespace IntermediateFile
{
    bool Builder(
        const FClassAnalysis& inClassAnalysis,
        std::vector<std::string>& outAnalysisRawH,
        std::vector<std::string>& outAnalysisRawCpp
    );
}
