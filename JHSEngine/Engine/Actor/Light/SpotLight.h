#pragma once
#include "../Core/ActorObject.h"
#include "../../Component/Light/SpotLightComponent.h"

class GSpotLight :public GActorObject
{
    typedef GActorObject Super;
    CVARIABLE()
    CSpotLightComponent* SpotLightComponent;
public:
    GSpotLight();

    virtual void Tick(float deltaTime);
public:
    void SetPosition(const XMFLOAT3& inPosition);
    void SetRotation(const fvector_3d& inRotation);
    void SetScale(const fvector_3d& inScale);
    void SetLightIntensity(const fvector_3d& inLightIntensity);
    void SetStartAttenuation(const float inStartAttenuation);
    void SetEndAttenuation(const float inEndAttenuation);
};