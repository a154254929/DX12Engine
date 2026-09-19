#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../EngineType.h"

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
        T *inArray = new T();
        inArray->SetOwner(this);
        ActorObjects.push_back(inArray);
        
        char objectName[128] = {0};
        sprintf(objectName, "ActorObject_%d", ActorObjects.size());
        inArray->Rename(objectName);

        return inArray;
    }
    
public:
    bool LineTraceBySingle(FCollisionResult& outHitResult, fvector_3d inStart, fvector_3d inEnd);
    
    const vector<GActorObject*>& GetAllActors() const { return ActorObjects; }

protected:
    CVARIABLE()
    GCamera* camera;
    
    CVARIABLE()
    vector<GActorObject*> ActorObjects;

};
