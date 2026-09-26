#pragma once
#include "Core/OperationHandleBase.h"


class GMoveArrow : public GOperationHandleBase
{
    typedef GOperationHandleBase Super;
public:
    GMoveArrow();
    
    void CreateMesh();
};
