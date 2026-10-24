#pragma once

class CComponent;
class CCoreMinimalObject;
namespace ComponentConstruction
{
    void UpdateConstructionComponent(CComponent* inComponent, CComponent* inParentComponent);

    void CreateComponent(CCoreMinimalObject* inOwner, CCoreMinimalObject* newObject);
}
