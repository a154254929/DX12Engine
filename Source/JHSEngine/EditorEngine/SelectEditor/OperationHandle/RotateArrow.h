#pragma once
#include "Core/OperationHandleBase.h"


class GRotateArrow : public GOperationHandleBase
{
    typedef GOperationHandleBase Super;
public:
    GRotateArrow();
    
    void CreateMesh(); 
    
public:
    virtual void SetScale(const fvector_3d& inScale);
    
public:
    virtual void Tick(float deltaTime);
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    virtual void OnCaptureKeyboardInformation(const FInputKey& inputKey);
    
protected:
    float lastT1Value = 0.f;
};
