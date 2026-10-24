#pragma once

#include "../../Core/CoreObject/CoreMinimalObject.h"

class CComponent : public CCoreMinimalObject
{
public:
    CComponent();
    
    void SetParentComponent(CComponent* inParent);
    void AddChildComponent(CComponent* inChild);
protected:
    CComponent* parentComponent;
    std::vector<CComponent*> childComponents;
};