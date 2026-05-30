#pragma once
#include <utility>

class CCoreMinimalObject;

template<class T>
T* CreateObject(CCoreMinimalObject* newnObject)
{
    return dynamic_cast<T*>(newnObject);
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(ParamTypes &&... params)
{
    return CreateObject<T>(new T(params...));
}