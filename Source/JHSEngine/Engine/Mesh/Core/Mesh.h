#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Interface/DirectXDeviceInterface.h"

class CMeshComponent;
class CMaterial;
enum EMeshRenderLayerType;
class GMesh
    : public GActorObject,
    public IRenderingInterface,
    public IDirectXDeviceInterface
{
    typedef GActorObject Super;
    
    CVARIABLE()
    CMeshComponent* meshComponent;

public:
    GMesh();
    virtual void Init();

    virtual void PreDraw(float deltaTime);
    virtual void Draw(float deltaTime);
    virtual void PostDraw(float deltaTime);
    
    virtual void SetPickup(bool inPickup);

public:
    virtual CMeshComponent * GetMeshComponent() { return meshComponent; }
    
    template<class T>
    T* GetMeshComponent()
    {
        return dynamic_cast<T*>(meshComponent);
    }
    
public:
    virtual void SetRenderLayerType(EMeshRenderLayerType inMeshRenderLayerType);
    
    void SetCastShadows(bool inCastShadows);
    bool IsCastShadow() const;
protected:
    virtual void SetMeshComponent(CMeshComponent* inMeshComponent);
public:
    UINT GetMaterialNum() const;
    
    vector<CMaterial*>* GetMaterials();

    // 重写位置/旋转/缩放，同步到 meshComponent
    virtual void SetPosition(const XMFLOAT3& inPosition) override;
    virtual void SetRotation(const fvector_3d& inRotation) override;
    virtual void SetScale(const fvector_3d& inScale) override;
};
