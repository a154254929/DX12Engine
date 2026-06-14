#pragma once
#include "../../Component/Sky/FogComponent.h"
#include "../Core/ActorObject.h"

class CFogComponent;

class GFog : public GActorObject
{
    typedef GActorObject Super;
public:
    GFog();
    CVARIABLE()
    CFogComponent* fogComponent;
    
    void SetFogColor (const fvector_color& inColor);
    void SetFogStart (float inStart);
    void SetFogRange (float inRange);
    
public:
    FORCEINLINE fvector_color GetFogColor () const {return fogComponent->GetFogColor();}
    FORCEINLINE float GetFogStart () const {return fogComponent->GetFogStart();}
    FORCEINLINE float GetFogRange () const {return fogComponent->GetFogRange();}
    
public:
};
