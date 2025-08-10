#pragma once
#ifdef _WIN32
#include "../../Platform/Windows/WindowsPlatform.h"
#endif
#include "../../EngineMinimal.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"

//提供核心对象
class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	virtual void BeginInit() {};
	virtual void Tick(float deltaTime) {};

	template<class ObjectType>
	static ObjectType* CreateObject(CCoreMinimalObject* newObject)
	{
		return dynamic_cast<ObjectType*>(newObject);
	}
	bool IsTick()const { return bTick; }
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> gObjects;