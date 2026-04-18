#pragma once
#include "LightType.h"
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterface.h"

class CMeshComponent;
class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
    typedef CTransformationComponent Super;
    CVARIABLE()
    CMeshComponent* lightMesh;
public:
    CLightComponent();
    ~CLightComponent();
    
    CMeshComponent* GetLightMesh(){return lightMesh;}
    
    const fvector_3d GetLightInstensity(){return lightInstensity;}
    void SetLightInstansity(const fvector_3d& inLightIntensity){lightInstensity = inLightIntensity;}
    
    const ELightType GetLightType(){return lightType;}
    
    void SetPosition(const XMFLOAT3& inPosition);
    void SetRotation(const fvector_3d& inRotation);
    void SetScale(const fvector_3d& inScale);
    void SetForwardVector(const XMFLOAT3& inForwardVector);
    void SetRightVector(const XMFLOAT3& inRightVector);
    void SetUpVector(const XMFLOAT3& inUpVector);
protected:
    void SetLightMesh(CMeshComponent* inLightMesh);
    
protected:
    fvector_3d lightInstensity;
    ELightType lightType;
};
