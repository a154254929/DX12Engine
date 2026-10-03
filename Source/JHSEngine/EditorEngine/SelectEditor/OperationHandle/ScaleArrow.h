#pragma once
#include "Core/OperationHandleBase.h"


class GScaleArrow : public GOperationHandleBase
{
    typedef GOperationHandleBase Super;
public:
    GScaleArrow();
    
    void CreateMesh(); 
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    
protected:
    fvector_3d relativePosition;
};
