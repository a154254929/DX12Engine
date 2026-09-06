#include "simple_library/public/simple_library.h"
#include "CodeReflection/CollectClassInfo/CollectClassInfo.h"

int main()
{
    char path[] = "D:\\JHSEngine\\JHSEngine\\EngineBuildTool\\TestObject.h";
    FClassAnalysis classAnalysis;
    CollectClassInfo::Collection(path, classAnalysis);
    return 0;
}