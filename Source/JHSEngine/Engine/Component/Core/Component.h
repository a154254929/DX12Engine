#pragma once

#include "../../Core/CoreObject/CoreMinimalObject.h"

class CComponent : public CCoreMinimalObject
{
public:
    CComponent();
    
    void SetParentComponent(CComponent* inParent);
    void AddChildComponent(CComponent* inChild);
    
public:
    FORCEINLINE std::vector<CComponent*>& GetChildren() { return childComponents; }
protected:
    CComponent* parentComponent;
    std::vector<CComponent*> childComponents;
};