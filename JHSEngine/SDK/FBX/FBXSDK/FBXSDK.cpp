#include "FBXSDK.h"
#include "include/fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libfbxsdk-mt.lib")

int main()
{
    FbxManager* manager = FbxManager::Create();
    
    return 0;
}