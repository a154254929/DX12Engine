#pragma once
#include "../Core/ActorObject.h"
#include "../../Component/Light/PointLightComponent.h"

class GPointLight :public GActorObject
{
    typedef GActorObject Super;
    CVARIABLE()
    CPointLightComponent* PointLightComponent;
public:
    GPointLight();

    virtual void Tick(float deltaTime);
public:
    void SetPosition(const XMFLOAT3& inPosition);
    void SetRotation(const fvector_3d& inRotation);
    void SetScale(const fvector_3d& inScale);
    void SetLightIntensity(const fvector_3d& inLightIntensity);
    void SetStartAttenuation(const float inStartAttenuation);
    void SetEndAttenuation(const float inEndAttenuation);
};