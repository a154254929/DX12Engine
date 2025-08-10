#pragma once
#ifdef _WIN32
#include "../../Platform/Windows/WindowsPlatform.h"
#endif
#include "../../CodeReflection/CodeReflectionMacroTag.h"

class CCoreMinimalObject
{
public:
	CCoreMinimalObject() {};
	virtual ~CCoreMinimalObject() {};

	template<class ObjectType>
	static ObjectType* CreateObject(CCoreMinimalObject* newObject)
	{
		return dynamic_cast<ObjectType*>(newObject);
	}
};