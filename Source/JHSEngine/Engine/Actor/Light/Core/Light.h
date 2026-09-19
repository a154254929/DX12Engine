#pragma once
#include "../../Core/ActorObject.h"
#include "../../../Component/Light/Core/LightComponent.h"

class GLight :public GActorObject
{
    typedef GActorObject Super;
    CVARIABLE()
    CLightComponent* lightComponent;
public:
    GLight();

    virtual void Tick(float deltaTime);
public:
    void SetPosition(const XMFLOAT3& inPosition);
    void SetRotation(const fvector_3d& inRotation);
    void SetScale(const fvector_3d& inScale);
    void SetLightIntensity(const fvector_3d& inLightIntensity);
    fvector_3d GetLightIntensity() const;
    
protected:
    virtual void SetLightComponent(CLightComponent* inComponent);
    
public:
    const CLightComponent* GetLightComponent() const { return lightComponent; }
};