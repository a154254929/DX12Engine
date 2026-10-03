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
    
public:
    void DisplaySelectOperationHandle();
    void HideSelectOperationHandle();
    
public:
    //设置新的手柄
    void SetNewSelectOperationHandle(GActorObject* inSelectOperationHandle);
    void SetNewSelectObject(GActorObject* inSelectObject);
private:
    static FOperationHandleSelectManager* instance;
    
    GActorObject* selectOperationHandle = nullptr;
};
#endif
