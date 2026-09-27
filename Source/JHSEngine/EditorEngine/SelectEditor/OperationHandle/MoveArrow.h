#pragma once
#include "Core/OperationHandleBase.h"


class GMoveArrow : public GOperationHandleBase
{
    typedef GOperationHandleBase Super;
public:
    GMoveArrow();
    
    void CreateMesh(); 
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    
    bool GetRayInterHitPosition(int x, int y, fvector_3d& outHitPosition);
    
protected:
    fvector_3d relativePosition;
};
