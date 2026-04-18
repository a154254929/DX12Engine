#include "LightManager.h"

void CLightManager::AddLight(CLightComponent* inLightInstance)
{
    for(auto light : lights)
    {
        if (light == inLightInstance)
            return;
    }
    lights.push_back(inLightInstance);
}

void CLightManager::RemoveLight(CLightComponent* inLightInstance)
{
    for(auto it = lights.begin(); it != lights.end(); ++it)
    {
        if (*it == inLightInstance)
            lights.erase(it);
    }
}
