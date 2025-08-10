#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	camera = CreateObject<CCamera>(new CCamera());
}
