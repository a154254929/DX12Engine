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
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    virtual void OnCaptureKeyboardInformation(const FInputKey& inputKey);
    
protected:
    fvector_3d relativePosition;
};
