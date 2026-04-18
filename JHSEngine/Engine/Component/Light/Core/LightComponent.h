#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterface.h"

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
public:
    CLightComponent();
    ~CLightComponent();
};
