#include "CoreMinimalObject.h"

vector<CCoreMinimalObject*> gObjects;

CCoreMinimalObject::CCoreMinimalObject()
    : owner(nullptr)
{
    bTick = true;
    gObjects.push_back(this);
}

CCoreMinimalObject::~CCoreMinimalObject()
{
    for (auto iter = gObjects.begin(); iter != gObjects.end(); ++iter)
    {
        if (*iter == this)
        {
            gObjects.erase(iter);
            break;
        }
    }
}
