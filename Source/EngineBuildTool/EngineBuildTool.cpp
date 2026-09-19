#include "simple_library/public/simple_library.h"
#include "CodeReflection/CollectClassInfo/CollectClassInfo.h"
#include "CodeReflection/IntermediateFile/SpawnIntermediateFile.h"
#include "EngineBuildTool.h"

int main()
{
    char path[] = "D:\\JHSEngine\\JHSEngine\\EngineBuildTool\\TestObject.h";
    FClassAnalysis classAnalysis;
    CollectClassInfo::Collection(path, classAnalysis);
    
    std::vector<std::string> outAnalysisRawH;
    std::vector<std::string> outAnalysisRawCpp;
    IntermediateFile::Builder(classAnalysis, outAnalysisRawH, outAnalysisRawCpp);
    return 0;
}
