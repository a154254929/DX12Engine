#pragma once
#include "Engine/Actor/Core/ActorObject.h"
#include "Engine/Interface/DirectXDeviceInterface.h"

class GOperationHandleBase :
    public GActorObject,
    public IDirectXDeviceInterface
{
    typedef GActorObject Super;
public:
    GOperationHandleBase();
};