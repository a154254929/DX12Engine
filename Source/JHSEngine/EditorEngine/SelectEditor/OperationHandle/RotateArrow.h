#pragma once
#include "Core/OperationHandleBase.h"

class CPlaneMeshComponent;
class GRotateArrow : public GOperationHandleBase
{
    typedef GOperationHandleBase Super;
    
    CVARIABLE()
    CPlaneMeshComponent* xPlaneComponent;
    
    CVARIABLE()
    CPlaneMeshComponent* yPlaneComponent;
    
    CVARIABLE()
    CPlaneMeshComponent* zPlaneComponent;
    
public:
    GRotateArrow();
    
    virtual void SetMeshRenderLayerType(EMeshRenderLayerType inRenderLayerType);
    
    void CreateMesh(); 
    
    void LoadPlaneComponentMaterial(
        CMeshComponent* inMeshComponent,
        const std::string& inFilename,
        const fvector_3d& inBaseColor
    );
    
protected:
    void SetCDValue(CMeshComponent* inMeshComponent, float inCDValue);
    void SetCDValue(float inCDValue);
    void ResetCDValue();
    
public:
    virtual void SetScale(const fvector_3d& inScale);
    virtual void SetPosition(const XMFLOAT3& inPosition);
    
public:
    virtual void ResetVisible();
    virtual void ResetVisible(CMeshComponent* inAxisComponent, bool inVisible);
    
public:
    virtual void BeginInit();
    virtual void Tick(float deltaTime);
    
    virtual void SetVisible(bool inVisible);
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    virtual void OnCaptureKeyboardInformation(const FInputKey& inputKey);
    
protected:
    float lastT1Value = 0.f;
};
