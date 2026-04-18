#pragma once
#include "../Core/ActorObject.h"
#include "../../Component/Light/ParallelLightComponent.h"

class GParallelLight :public GActorObject
{
    typedef GActorObject Super;
    CVARIABLE()
    CParallelLightComponent* parallelLightComponent;
public:
    GParallelLight();
    void SetPosition(const XMFLOAT3& inPosition);
    void SetRotation(const fvector_3d& inRotation);
    void SetScale(const fvector_3d& inScale);
};