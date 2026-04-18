#include "LightComponent.h"
#include "../../../Manager/LightManager.h"

CLightComponent::CLightComponent()
{
    GetLightManager()->AddLight(this);
}
CLightComponent::~CLightComponent()
{
    GetLightManager()->RemoveLight(this);
}