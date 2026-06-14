#pragma once
#include "../Core/Component.h"

class CFogComponent : public CComponent
{
    typedef CComponent Super;
public:
    CFogComponent();
    
    void SetFogColor (const fvector_color& inColor);
    void SetFogStart (float inStart);
    void SetFogRange (float inRange);
    void SetDirty (bool inDirty);
    
public:
    FORCEINLINE fvector_color GetFogColor () const {return fogColor;}
    FORCEINLINE float GetFogStart () const {return fogStart;}
    FORCEINLINE float GetFogRange () const {return fogRange;}
    FORCEINLINE bool IsDirty () const {return bDirty;}
    
protected:
    fvector_color fogColor;
    float fogStart;
    float fogRange;
    
    bool bDirty;
};
