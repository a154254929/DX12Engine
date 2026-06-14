#pragma onc
#include "../../Mesh/SphereMesh.h"

class GSky : public GSphereMesh
{
    typedef GSphereMesh Super;
public:
    GSky();
    
    virtual void Tick(float deltaTime);
};
