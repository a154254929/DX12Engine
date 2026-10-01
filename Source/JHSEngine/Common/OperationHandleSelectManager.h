#pragma once
#include "../Engine/EngineMacro.h"

#if EDITOR_ENGINE
class GActorObject;
class FOperationHandleSelectManager
{
public:
    FOperationHandleSelectManager();
    
    
    static FOperationHandleSelectManager* GetInstance();
    static void DestroyInstance();
    
private:
    static FOperationHandleSelectManager* instance;
    
    GActorObject* selectOperationHandle;
};
#endif
