#pragma once
#include "Core/OperationHandleBase.h"


class GMoveArrow : public GOperationHandleBase
{
    typedef GOperationHandleBase Super;
public:
    GMoveArrow();
    
    void CreateMesh(); 
    
protected:
    fvector_3d GetCustomAxisDirection(
        const fvector_3d inRayWorldOriginPosition,
        const fvector_3d inRayWorldDirection,
        const fvector_3d inObjectWorldPosition
    ) const;
    
protected:
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftButtonDown(int x, int y);
    virtual void OnLeftButtonUp(int x, int y);
    virtual void OnCaptureKeyboardInformation(const FInputKey& inputKey);
    
protected:
    fvector_3d relativePosition;
};
