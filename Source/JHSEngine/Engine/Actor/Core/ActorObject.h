#pragma once
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;
//G->Game
class GActorObject : public CCoreMinimalObject
{
    CVARIABLE()
    CTransformationComponent* rootComponent;

public:
    GActorObject();

    FORCEINLINE CTransformationComponent* GetRootComponent() { return rootComponent; };
    
    BoundingBox GetBoundingBox();
    
    void GetBoundingBox(BoundingBox& outBoundingBox);

public:
    virtual void SetPosition(const XMFLOAT3& inPosition);
    virtual void SetRotation(const fvector_3d& inRotation);
    virtual void SetRotation(const frotator& inRotator);
    virtual void SetRotationQuat(const fquat& inRotatorQuat);
    virtual void SetScale(const fvector_3d& inScale);
    
    virtual void SetPickup(bool inPickup);

    XMFLOAT3& GetPosition();
    frotator GetRotation() const;
    fquat GetRotationQuat() const;
    fvector_3d GetScale() const;

    XMFLOAT3& GetForwardVector();
    XMFLOAT3& GetRightVector();
    XMFLOAT3& GetUpVector();
};