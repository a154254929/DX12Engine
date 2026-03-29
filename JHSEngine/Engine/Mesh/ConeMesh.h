#pragma once
#include "Core/Mesh.h"

class GConeMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float deltaTime);

    void CreateMesh(float inRadius, float inHeight, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);
};