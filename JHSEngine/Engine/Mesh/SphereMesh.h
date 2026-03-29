#pragma once
#include "Core/Mesh.h"

class GSphereMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float deltaTime);

    void CreateMesh(float inRadius, uint32_t inAxialSubdivision, uint32_t inHeightSubdivision);
};