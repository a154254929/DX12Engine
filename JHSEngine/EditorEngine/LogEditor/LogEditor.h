#pragma once
#include "../../Engine/Core/Engine.h"
#include "../Core/EditorBase.h"

class FLogEditor : public FEditorBase
{
public:
    virtual void BuildEditor();
    virtual void DrawEditor(float deltaTime);
    virtual void ExitEditor();
};