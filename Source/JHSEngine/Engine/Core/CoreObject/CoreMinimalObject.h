#pragma once
#ifdef _WIN32
#include "../../Platform/Windows/WindowsPlatform.h"
#endif
#include "../../EngineMinimal.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../Consttruction/ObjectConstruction.h"

//提供核心对象
class CCoreMinimalObject : public IGuidInterface
{
public:
    CCoreMinimalObject();
    virtual ~CCoreMinimalObject();

    virtual void BeginInit() {};
    virtual void Tick(float deltaTime) {};

    template<class ObjectType>
    static ObjectType* CreateObject(const FCreateObjectParam inOwner, CCoreMinimalObject* newObject)
    {
        ObjectType* outObj = dynamic_cast<ObjectType*>(newObject);
        outObj->SetOwner(inOwner.owner);
        return outObj;
    }
    bool IsTick()const { return bTick; }
    
    FORCEINLINE CCoreMinimalObject* GetOwner() const { return owner; }
    std::string GetName() { return name; }
    void SetOwner(CCoreMinimalObject* newOwner) { owner = newOwner; }
    void Rename(const std::string& newName) { name = newName; }
protected:
    bool bTick;
    
    CCoreMinimalObject* owner;
    std::string name;
};

extern vector<CCoreMinimalObject*> gObjects;