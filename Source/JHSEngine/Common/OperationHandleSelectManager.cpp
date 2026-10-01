#include "OperationHandleSelectManager.h"

FOperationHandleSelectManager* FOperationHandleSelectManager::instance = nullptr;

FOperationHandleSelectManager* FOperationHandleSelectManager::GetInstance()
{
    if (!instance)
    {
        instance = new FOperationHandleSelectManager();
    }
    return instance;
}

void FOperationHandleSelectManager::DestroyInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}
