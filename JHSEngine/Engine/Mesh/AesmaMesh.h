#pragma once
#include "Core/Mesh.h"

class GAesmaMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float deltaTime);

    void CreateMesh(
        float inOuterRadius,
        float inInnerRadius,
        float inHeight,
        uint32_t inAxialSubdivision,
        uint32_t inHeightSubdivision
    );
};