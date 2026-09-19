#pragma once
#include "ActorMeshConstruction.h"

#define CREATE_RENDER_DATA(T, ...)\
    ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManager(), this, __VA_ARGS__)

//##args