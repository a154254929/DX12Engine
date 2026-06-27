#pragma once
#include "../Engine.h"
#include "Viewport.h"
#include "../../Actor/Core/ActorObject.h"

class GClientViewport
    : public GActorObject
    , public FViewport
{
    typedef GActorObject SuperA;
    typedef FViewport SuperV;
public:
    GClientViewport();
    
    //设置视锥体
    void SetFrustum(
        float inYFov,
        float inAspect,
        float inZNear,
        float inZFar
    );
    
    bool LookAt(
        const fvector_3d &inPosition,
        const fvector_3d &inTargetPosition,
        const fvector_3d &inUp = fvector_3d(0.f, 1.f, 0.f)
    );
    
    virtual void Tick(float deltaTime);
    
    virtual void BuildViewMatrix(float deltaTime);
    
public:
    FORCEINLINE float GetYFov() { return yFov; }
    FORCEINLINE float GetAspect() { return aspect; }
    FORCEINLINE float GetZNear() { return zNear; }
    FORCEINLINE float GetZFar() { return zFar; }
    FORCEINLINE bool GetDirty() { return bDirty; }
    
    
    void SetDirty(bool inDirty) { bDirty = inDirty; }
    
private:
    float yFov;
    float aspect;
    float zNear;
    float zFar;

    bool bDirty;
};