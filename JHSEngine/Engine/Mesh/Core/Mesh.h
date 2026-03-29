#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Interface/DirectXDeviceInterface.h"

class CMeshComponent;
class CMaterial;
class GMesh
    : public GActorObject,
    public IRenderingInterface,
    public IDirectXDeviceInterface
{
    CVARIABLE()
    CMeshComponent* meshComponent;

public:
    GMesh();
    virtual void Init();

    virtual void PreDraw(float deltaTime);
    virtual void Draw(float deltaTime);
    virtual void PostDraw(float deltaTime);

public:
    virtual CMeshComponent * GetMeshComponent() { return meshComponent; }
protected:
    virtual void SetMeshComponent(CMeshComponent* inMeshComponent);
public:
    UINT GetMaterialNum() const;
    
    vector<CMaterial*>* GetMaterials();

    // 重写位置/旋转/缩放，同步到 meshComponent
    void SetPosition(const XMFLOAT3& inPosition);
    void SetRotation(const fvector_3d& inRotation);
    void SetScale(const fvector_3d& inScale);
};