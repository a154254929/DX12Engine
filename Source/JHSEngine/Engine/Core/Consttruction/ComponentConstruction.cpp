#include "ComponentConstruction.h"
#include "Engine/Actor/Core/ActorObject.h"
#include "Engine/Component/TransformationComponent.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "Engine/Component/Core/Component.h"


namespace ComponentConstruction
{
    void UpdateConstructionComponent(CComponent* inComponent, CComponent* inParentComponent)
    {
        if (inComponent != nullptr && inParentComponent != nullptr && inComponent != inParentComponent)
        {
            inParentComponent->AddChildComponent(inComponent);
            inComponent->SetParentComponent(inParentComponent);
        }
    }

    void CreateComponent(CCoreMinimalObject* inOwner, CCoreMinimalObject* newObject)
    {
        if (CComponent* outComp = dynamic_cast<CComponent*>(newObject))
        {
            if (inOwner)
            {
                if (CComponent* ownerComponent = dynamic_cast<CComponent*>(inOwner))
                {
                    UpdateConstructionComponent(outComp, ownerComponent);
                }
                else if (GActorObject* ownerActor = dynamic_cast<GActorObject*>(inOwner))
                {
                    UpdateConstructionComponent(outComp, ownerActor->GetRootComponent());
                }
            }
        }
    }
    
}