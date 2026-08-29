#pragma once
#include <utility>

class CCoreMinimalObject;
#include "../../Core/CoreObject/CoreMinimalObject.h"

struct FCreateObjectParam
{
    FCreateObjectParam()
        : owner(nullptr)
    {
    }
    
    CCoreMinimalObject* owner;
};

template<class T>
T* CreateObject(const FCreateObjectParam& inOwner, CCoreMinimalObject* newObject)
{
    T* outObj = dynamic_cast<T*>(newObject);
    outObj->SetOwner(inOwner.owner);
    return outObj;
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(const FCreateObjectParam& inOwner, ParamTypes &&... params)
{
    return CreateObject<T>(inOwner, new T(params...));
}