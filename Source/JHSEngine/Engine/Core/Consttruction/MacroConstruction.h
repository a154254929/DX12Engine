#pragma once
#include "ActorMeshConstruction.h"

#define CREATE_RENDER_DATA(T, ...)\
    ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManager(), this, __VA_ARGS__)

#define CREATE_RENDER_DATA_BY_COMPONENT(T, Component, ...)\
    ActorMeshConstruction::CreateMeshRenderDataByComponent<T>(GetMeshManager(), Component, __VA_ARGS__)

//##args