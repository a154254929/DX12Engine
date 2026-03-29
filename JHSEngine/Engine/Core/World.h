#pragma once
#include "CoreObject/CoreMinimalObject.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;

class CWorld : public CCoreMinimalObject
{
public:
    CWorld();
    GCamera* GetCamera() { return camera; };

    template<class T>
    T *CreateActorObject()
    {
        T *InArray = new T();
        ActorObjects.push_back(InArray);

        return InArray;
    }

protected:
    CVARIABLE()
    GCamera* camera;
    
    CVARIABLE()
    vector<GActorObject*> ActorObjects;

};
