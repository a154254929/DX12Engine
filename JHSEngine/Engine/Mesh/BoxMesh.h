#pragma once
#include "Core/Mesh.h"

class GBoxMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float deltaTime);

    void CreateMesh(float inHeight, float inWidth, float inDepth);
};