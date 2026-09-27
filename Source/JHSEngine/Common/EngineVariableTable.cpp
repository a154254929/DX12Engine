#include "EngineVariableTable.h"
#include "../Engine/EngineMacro.h"

#if EDITOR_ENGINE

int actorSelectId = 0;
class GActorObject* selectedObject = nullptr;
class CMeshComponent* selectedAxisComponent = nullptr;

class GMoveArrow* moveArrow = nullptr;

#endif