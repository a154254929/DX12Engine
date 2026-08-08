#pragma once
#include "../Core/CoreObject/CoreMinimalObject.h"

class CLightComponent;

class CLightManager : public CCoreMinimalObject
{
    friend class FGeometryMap;
public:
    void AddLight(CLightComponent* inLightInstance);
    void RemoveLight(CLightComponent* inLightInstance);
    
    const vector<CLightComponent*>& GetLights() {return lights;}
protected:
    vector<CLightComponent*> lights;
};
