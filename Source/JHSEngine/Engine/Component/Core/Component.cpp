#include "Component.h"

CComponent::CComponent()
{
    parentComponent = nullptr;
}

void CComponent::SetParentComponent(CComponent* inParent)
{
    parentComponent = inParent;
}

void CComponent::AddChildComponent(CComponent* inChild)
{
    if (inChild)
    {
        inChild->parentComponent = this;
        childComponents.push_back(inChild);
    }
}
