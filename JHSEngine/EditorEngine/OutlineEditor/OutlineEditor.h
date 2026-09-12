#pragma once
#include "../Core/EditorBase.h"
#include "../../Engine/Interface/DirectXDeviceInterface.h"

class GActorObject;

class FOutlineEditor : public FEditorBase, public IDirectXDeviceInterface
{
public:
    virtual void BuildEditor();
    virtual void DrawEditor(float deltaTime);
    virtual void ExitEditor();
    
    void HighlightDisplayObject(GActorObject* inActorObject);
};
